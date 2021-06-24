#include "clad/Differentiator/ErrorEstimator.h"
#include "clad/Differentiator/DerivativeBuilder.h"
#include "clad/Differentiator/DiffPlanner.h"
#include "clad/Differentiator/ReverseModeVisitor.h"

#include "clang/AST/Decl.h"

using namespace clang;

namespace clad {
  void ErrorEstimationHandler::SetErrorEstimationModel(
      FPErrorEstimationModel* estModel) {
    m_EstModel = estModel;
  }

  DeclWithContext
  ErrorEstimationHandler::Calculate(const clang::FunctionDecl* FD,
                                    const DiffRequest& request) {
    FD = FD->getDefinition();
    DeclWithContext result{};

    // Call gradient on the function as we would normally.
    ReverseModeVisitor V(m_builder);
    result = V.Derive(FD, request);

    return result;
  }

  bool ErrorEstimationHandler::RegisterVariable(VarDecl* VD) {
    // Get the types on the declartion and initalization expression.
    QualType varDeclBase = VD->getType();
    const Type *varDeclType =
        varDeclBase->isArrayType()
            ? varDeclBase->getArrayElementTypeNoTypeQual()
            : varDeclBase.getTypePtr();
    const Expr* init = VD->getInit();
    // If declarationg type in not floating point type, we want to do two things.
    if (!varDeclType->isFloatingType()) {
      // Firstly, we want to check if the declaration is a lossy conversion.
      // For example, if we have something like:
      // double y = 2.77788;
      // int x = y <-- This causes truncation in y,
      // making _delta_x = y - (double)x
      // For now, we will just warn the user of casts like these
      // because we assume the cast is intensional.
      if (init && init->IgnoreImpCasts()->getType()->isFloatingType())
        m_VBase.diag(
            DiagnosticsEngine::Warning,
            VD->getEndLoc(),
            "Lossy assignment from '%0' to '%1', this error will not be "
            "taken into cosideration while estimation",
            {init->IgnoreImpCasts()->getType().getAsString(),
             varDeclBase.getAsString()});
      // Secondly, we want to only register floating-point types
      // So return false here.
      return false;
    }
    // Next, we want to check if there is an assignment that leads to 
    // truncation, for example, something like so
    // double y = ...some double value...
    // float x = y; <-- This leads to rounding of the lower bits
    // making _delta_x = y - (double)x
    // For now, we shall just warn against such assignments...
    // FIXME: figure how to do this out elegantly.

    // Now, we can register the variable.
    // So return true here.
    return true;
  }

  DeclRefExpr* ErrorEstimationHandler::GetUnderlyingDeclRefOrNull(Expr* expr) {
    // First check if it is an array subscript expression.
    ArraySubscriptExpr* temp =
        dyn_cast<ArraySubscriptExpr>(expr->IgnoreImplicit());
    // The see if it is convertiable to a DeclRefExpr.
    if (temp)
      return dyn_cast<DeclRefExpr>(temp->getBase()->IgnoreImplicit());
    else
      return dyn_cast<DeclRefExpr>(expr->IgnoreImplicit());
  }

  Expr* ErrorEstimationHandler::GetParamReplacement(const ParmVarDecl* VD) {
    auto it = m_ParamRepls.find(VD);
    if (it != m_ParamRepls.end())
      return it->second;
    return nullptr;
  }

} // namespace clad
