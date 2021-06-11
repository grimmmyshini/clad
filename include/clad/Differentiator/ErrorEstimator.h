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
  /// Keeps track of the push expressions to add to forward mode post visitation.
  /// Also heps maintain readability
  VisitorBase::Stmts m_pushExpr;
  /// Reference to the final error parameter in the augumented target function
  clang::Expr* m_FinalError;
  /// Reference to the return error expression
  clang::Expr* m_RetErrorExpr = nullptr;
  /// A reference to the builder instance so that we can call Derive
  DerivativeBuilder& m_builder;
  /// A instance of visitor base to utilize all functionalities from the same 
  // since we do not derive from it
  VisitorBase m_VBase;
  /// An instance of the custom error estimation model to be used
  EstimationModel* m_EstModel;
  /// A set of assignments resulting for declaration statments
  VisitorBase::Stmts m_ForwardReplStmts;
  /// A vector to keep track of error statements for delayed emission
  VisitorBase::Stmts m_ReverseErrorStmts;
  /// Keeps track of the tape info for the estimation variables
  /// so that we can use the correct tape and value in the case of
  /// re-assignments, loops etc.
  /// std::unordered_map<const clang::VarDecl*, TapeInfo> m_ReplaceEstVar;

public:
  ErrorEstimationHandler(DerivativeBuilder& builder)
      : m_builder(builder), m_VBase(VisitorBase(builder)) {
  }
  ~ErrorEstimationHandler() {}
  /// \brief Function to calculate the estimated error in a function.
  /// This function internally calls Derive
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
  bool RegisterVariable(clang::VarDecl* VD);
  /// \brief Calculate aggregate error from m_EstimateVar.
  /// Builds the final error estimation statement
  clang::Stmt* CalculateAggregateError();
  /// \brief Get the underlying DeclRefExpr type it it exists
  /// \param[in] expr The expression whose DeclRefExpr is requested 
  /// \returns The DeclRefExpr of input or null
  clang::DeclRefExpr* GetUnderlyingDeclRefOrNull(clang::Expr* expr);

  friend class ReverseModeVisitor;
};

} // namespace clad

#endif // CLAD_ERROR_ESTIMATOR_H