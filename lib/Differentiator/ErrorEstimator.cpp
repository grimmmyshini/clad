#include "clad/Differentiator/DerivativeBuilder.h"
#include "clad/Differentiator/DiffPlanner.h"
#include "clad/Differentiator/ErrorEstimator.h"


#include "clang/AST/Decl.h"

using namespace clang;

namespace clad {

DeclWithContext ErrorEstimationHandler::Calculate(const clang::FunctionDecl* FD,
                                                  const DiffRequest& request) {
  FD = FD->getDefinition();
  DeclWithContext result{};

  // If not custom model for provided by user, use the built in Taylor
  // approximation
  if (!request.CustomModel)
    m_EstModel = new TaylorApprox();
  else
    m_EstModel = request.CustomModel;
  m_EstModel->m_VBase = &m_VBase;

  // Call gradient on the function as we would normally
  ReverseModeVisitor V(m_builder);
  result = V.Derive(FD, request);

  return result;
}

// FIXME: There is probably more to this...
VarDecl* ErrorEstimationHandler::RegisterVariable(VarDecl* VD) {
  // Get the types on the declartion and initalization expression
  QualType varDeclType = VD->getType();
  const Expr* init = VD->getInit();
  // If declarationg type in not floating point type, we want to do two things
  if (!varDeclType->isFloatingType()) {
    // Firstly, we want to check if the declaration is a lossy conversion
    // For example, if we have something like
    // double y = 2.77788;
    // int x = y <-- This causes truncation in y,
    // making _delta_x = y - (double)x
    // For now, we will just warn the user of casts like these
    if (init && init->IgnoreImpCasts()->getType()->isFloatingType())
      m_VBase.diag(DiagnosticsEngine::Warning, VD->getEndLoc(),
                   "Lossy assignment from '%0' to '%1', this error will not be "
                   "taken into cosideration while estimation",
                   {init->IgnoreImpCasts()->getType().getAsString(),
                    VD->getType().getAsString()});
    // Secondly, we want to only register floating-point types
    return nullptr;
  }
  // Next, we want to check if there is an assignment that leads to truncation
  // For example, something like so
  // double y = ...some double value...
  // float x = y; <-- This leads to rounding of the lower bits
  // making _delta_x = y - (double)x
  // For now, we shall just warn against such assignments...
  // FIXME: figure how to do this out elegantly

  // Now, we can register the variable
  return m_EstModel->AddVarToEstimate(VD);
}

// For cases like x = y * (x++), it is likely we have to build a custom init
// along the way to make sure we do not end up modifying the results of the
// actual code. For eg.
// _t0 = y * (++x)
// x = y * (++x) the value of x is not what we exepect, this propogates errors
// _t1 = y * (++_t0); this is the correct way,
// for post inc/dec this is trickier because the change has to be propagated.
// But for now, we will do a simple find and replace
// FIXME: take care of above issue
clang::Stmt* ErrorEstimationHandler::UpdateReplacement(VarDecl* VD, Expr* init){
  // Firstly, build a brand new varible declaration statement that is the copy of the on above
  auto replacedDecl = m_VBase.BuildVarDecl(VD->getType(), "_r_" + VD->getNameAsString() + "_", init);
  auto replacedDeclStmt = m_VBase.BuildDeclStmt(replacedDecl);
  // Then update/create the replacement
  m_ReplaceEstVar[VD] = m_VBase.BuildDeclRef(replacedDecl);
  // Lastly return the stmt to be emitted into the code
  return replacedDeclStmt;
}

clang::Expr* ErrorEstimationHandler::GetReplacement(VarDecl* VD){
  auto it = m_ReplaceEstVar.find(VD);
  if(it != m_ReplaceEstVar.end())
    return it->second;
  return nullptr;
}
} // namespace clad