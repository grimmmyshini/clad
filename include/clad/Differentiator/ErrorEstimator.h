#ifndef CLAD_ERROR_ESTIMATOR_H
#define CLAD_ERROR_ESTIMATOR_H

#include "EstimationModel.h"

#include <utility>

namespace clang {
class Stmt;
class Expr;
class Decl;
class VarDecl;
class FunctionDecl;
class CompoundStmt;
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
class ErrorEstimationHandler {
  /// Keeps a track of the delta error expression we shouldn't emit 
  bool m_DoNotEmitDelta = false;
  /// Keeps a track of if the current variable's replacement should be emitted 
  /// Handle special cases like pre inc/dec operators. 
  /// TODO: Remove this and register variables on sight in VisitUnOp
  bool m_DoNotEmitRepl = false;
  // Keeps track of whether we are in a compound statement, if so, we need to
  // save the declaration of any variable whose scope is limited so that we
  // can use those values later.
  int m_InCompountStmt = 0;
  // Inside of a different scope, we
  llvm::SmallVector<clang::Stmt*, 16> m_EarlyDecls;
  /// Keeps track of the most recent replacements for the estimation variables
  /// so that we can use the correct value in the case of re-assignments
  std::unordered_map<const clang::VarDecl*, clang::Expr*> m_ReplaceEstVar;
  /// Reference to the final error parameter in the augumented target function
  clang::Expr* m_FinalError;
  /// A reference to the builder instance so that we can call Derive of visitor
  DerivativeBuilder& m_builder;
  /// A instance of visitor base to utilize all functionalities from the same 
  // since we do not derive from it
  VisitorBase m_VBase;
  /// An instance of the custom error estimation model to be used
  EstimationModel* m_EstModel;

public:
  ErrorEstimationHandler(DerivativeBuilder& builder)
      : m_builder(builder), m_VBase(VisitorBase(builder)) {
  }
  ~ErrorEstimationHandler() {}
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
  clang::VarDecl* RegisterVariable(clang::VarDecl* VD);
  /// \brief Calculate aggregate error from m_EstimateVar.
  /// Builds the final error estimation statement
  clang::Stmt* CalculateAggregateError();
  /// \brief Update the replacement of variable declrations.
  /// In the case of reassignments, we want to save the value of RHS
  /// previously encountered so that we can correctly use in error estimation
  /// \param[in] VD The variable declaration to update replacement for
  /// \param[in] init The initalizer for above variable declaration
  /// \returns statement to the full variable declaration of the replacement
  clang::Stmt* UpdateReplacement(clang::VarDecl* VD, clang::Expr* init = nullptr);
  /// \brief Get a declRefExpr of the replaced value for an estimate variable
  /// \param[in] VD The variable declaration to get the replacement for
  /// \returns A reference to the replaced declRefExpr
  clang::Expr* GetReplacement(clang::VarDecl* VD);

  friend class ReverseModeVisitor;
};

} // namespace clad

#endif // CLAD_ERROR_ESTIMATOR_H