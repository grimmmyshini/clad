#ifndef CLAD_ERROR_ESTIMATOR_TPP
#define CLAD_ERROR_ESTIMATOR_TPP

#ifndef CLAD_ERROR_ESTIMATOR_H
#error __FILE__ should only be included from ErrorEstimator.h.
#endif

#include "clad/Differentiator/DerivativeBuilder.h"
#include "clad/Differentiator/ErrorEstimator.h"

#include "clang/AST/Decl.h"

using namespace clang;

namespace clad {

template <class T>
DeclWithContext ErrorEstimationHandler<T>::Calculate(const clang::FunctionDecl* FD, const DiffRequest& request){

  // Any calls to Derive hereafter are done in the context of error estimation
  // hence we should emmit estimation code whenever we derive 
  m_EstimationInFlight = true;

  // Call gradient on the function as we would normally
  FD = FD->getDefinition();
  DeclWithContext result{};

  ReverseModeVisitor V(m_builder);
  result = V.Derive(FD, request);

  // Change the final call to reflect the absolute error
  // FIXME: Do this is reversemode derive

  return result;
}

// FIXME: There is probably more to this...
template <class T>
VarDecl* ErrorEstimationHandler<T>::RegisterVariable(const VarDecl* VD) {
  // Get the types on the declartion and initalization expression
  QualType varDeclType = VD->getType();
  QualType initType = VD->getInit()->IgnoreImpCasts()->getType();
  // If declarationg type in not floating point type, we want to do two things
  if (!varDeclType->isFloatingType()) {
    // Firstly, we want to check if the declaration is a lossy conversion
    // For example, if we have something like
    // double y = 2.77788;
    // int x = y <-- This causes truncation in y,
    // making _delta_x = y - (double)x
    // For now, we will just warn the user of casts like these
    if (initType->isFloatingType())
      m_VBase.diag(DiagnosticsEngine::Warning, VD->getBeginLoc(),
                   "Lossy assignment from '%0' to '%1'",
                   {VD->getInit()->getType().getAsString(),
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
  return m_EstModel.AddVarToEstimate(VD);
}
} // namespace clad

#endif // CLAD_ERROR_ESTIMATOR_TPP