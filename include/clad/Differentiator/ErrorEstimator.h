#ifndef CLAD_ERROR_ESTIMATOR_H
#define CLAD_ERROR_ESTIMATOR_H

#include "EstimationModel.h"

#include <utility>

namespace clang {
class Expr;
class Decl;
class VarDecl;
class FunctionDecl;
} // namespace clang

namespace clad {
struct DiffRequest;
class VisitorBase;
class DerivativeBuilder;
} // namespace clad

namespace clad {

/// A pair of FunctionDecl and potential enclosing context, e.g. a function
/// in nested namespaces
using DeclWithContext = std::pair<clang::FunctionDecl*, clang::Decl*>;

/// The estimation handler which interfaces with Clad's reverse mode visitors to
/// fetch derivatives
/// \tparam SubModel The user defined custom error estimation model
template <class SubModel> class ErrorEstimationHandler {
  /// Determines if an estimation is in process; helps decide whether
  /// to visit error estimation specific code in calls to clad::gradient
  bool m_EstimationInFlight;
  /// Keeps track of subexpression error; useful for error accumulation
  clang::Expr* m_SubExprErr;
  /// An instance of the custom error estimation model to be used
  EstimationModel<SubModel>& m_EstModel;
  /// A reference to the current visitor base instance
  VisitorBase& m_VBase;
  /// A reference to the builder instance so that we can call Derive of visitor
  DerivativeBuilder& m_builder;

public:
  ErrorEstimationHandler(DerivativeBuilder& builder)
      : m_builder(builder), m_VBase(VisitorBase(builder)) {}
  /// Workaround to be able to declare variable template of this class
  ErrorEstimationHandler();
  ~ErrorEstimationHandler();
  /// \brief Function to calculate the estimated error in a function.
  /// This function internally calls Derive and performs some housekeeping tasks
  /// required to set up error estimation
  /// \param[in] FD The function declaration on which estimation is performed
  /// \param[in] request The meta information about the kind of differentiation
  /// to be used for estimation
  /// \returns the new derivative function augmented
  /// with estimation code and its potential enclosing context
  DeclWithContext Calculate(const clang::FunctionDecl* FD,
                            const DiffRequest& request);
  /// \brief Register variables to be used while accumulating error.
  /// Register variable declarations so that they may be used while calculating
  /// the final error estimates. Any unregistered variables will not be
  /// considered for the final estimation.
  /// \param[in] VD The variable declaration to be registered
  /// \returns The Variable declaration of the '_delta_' prefixed variable
  clang::VarDecl* RegisterVariable(const clang::VarDecl* VD);

  friend class ReverseModeVisitor;
};

} // namespace clad

#include "ErrorEstimator.tpp"

#endif // CLAD_ERROR_ESTIMATOR_H