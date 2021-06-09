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
    m_EstModel = new TaylorApprox(m_builder);
  else
    m_EstModel = request.CustomModel;

  // Call gradient on the function as we would normally
  ReverseModeVisitor V(m_builder);
  result = V.Derive(FD, request);

  return result;
}

// FIXME: There is probably more to this...
bool ErrorEstimationHandler::RegisterVariable(VarDecl* VD) {
  // Get the types on the declartion and initalization expression
  const Type* varDeclType = VD->getType()->isArrayType()
                             ? VD->getType()->getArrayElementTypeNoTypeQual()
                             : VD->getType().getTypePtr();
  const Expr *init = VD->getInit();
  // If declarationg type in not floating point type, we want to do two things
  if (!varDeclType->isFloatingType()) {
    // Firstly, we want to check if the declaration is a lossy conversion
    // For example, if we have something like
    // double y = 2.77788;
    // int x = y <-- This causes truncation in y,
    // making _delta_x = y - (double)x
    // For now, we will just warn the user of casts like these
    // because we assume the cast is intensional 
    if (init && init->IgnoreImpCasts()->getType()->isFloatingType())
      m_VBase.diag(DiagnosticsEngine::Warning, VD->getEndLoc(),
                   "Lossy assignment from '%0' to '%1', this error will not be "
                   "taken into cosideration while estimation",
                   {init->IgnoreImpCasts()->getType().getAsString(),
                    VD->getType().getAsString()});
    // Secondly, we want to only register floating-point types
    return false;
  }
  // Next, we want to check if there is an assignment that leads to truncation
  // For example, something like so
  // double y = ...some double value...
  // float x = y; <-- This leads to rounding of the lower bits
  // making _delta_x = y - (double)x
  // For now, we shall just warn against such assignments...
  // FIXME: figure how to do this out elegantly

  // Now, we can register the variable
  return true;
}

ErrorEstimationHandler::TapeInfo ErrorEstimationHandler::GetReplacement(VarDecl* VD){
  auto it = m_ReplaceEstVar.find(VD);
  if(it != m_ReplaceEstVar.end())
    return it->second;
  return TapeInfo();
}
} // namespace clad