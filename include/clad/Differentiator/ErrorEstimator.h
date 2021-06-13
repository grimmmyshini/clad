#ifndef CLAD_ERROR_ESTIMATOR_H
#define CLAD_ERROR_ESTIMATOR_H

#include "EstimationModel.h"

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
  class FPErrorEstimationModel;
} // namespace clad

namespace clad {
  /// The estimation handler which interfaces with Clad's reverse mode visitors
  /// to fetch derivatives.
  class ErrorEstimationHandler {
    /// Keeps a track of the delta error expression we shouldn't emit.
    bool m_DoNotEmitDelta = false;
    /// Reference to the final error parameter in the augumented target
    /// function.
    clang::Expr* m_FinalError;
    /// Reference to the return error expression.
    clang::Expr* m_RetErrorExpr = nullptr;
    /// A reference to the builder instance so that we can call Derive.
    DerivativeBuilder& m_builder;
    /// A instance of visitor base to utilize all functionalities from the same
    // since we do not derive from it.
    VisitorBase m_VBase;
    /// An instance of the custom error estimation model to be used.
    FPErrorEstimationModel* m_EstModel; // We do not own this.
    /// A set of assignments resulting for declaration statments.
    VisitorBase::Stmts m_ForwardReplStmts;
    /// A vector to keep track of error statements for delayed emission.
    VisitorBase::Stmts m_ReverseErrorStmts;
    /// A set of declRefExprs for parameter value replacements.
    std::unordered_map<const clang::VarDecl*, clang::Expr*> m_paramRepls;

  public:
    ErrorEstimationHandler(DerivativeBuilder& builder)
        : m_builder(builder), m_VBase(VisitorBase(builder)) {}
    ~ErrorEstimationHandler() {}
    /// Function to set the error estimation model currently in use.
    /// \param[in] estModel The error estimation model, can be either
    /// an in-built one (TaylorApprox) or one provided by the user.
    void SetErrorEstimationModelInUse(FPErrorEstimationModel* estModel);
    /// Function to calculate the estimated error in a function.
    /// This function internally calls ReverseModeVisitor::Derive.
    /// \param[in] FD Function declaration on which estimation is performed.
    /// \param[in] request The meta information about the kind of
    /// differentiation to be used for estimation. 
    /// \returns the new derivative function augmented with estimation code 
    /// and its potential enclosing context.
    DeclWithContext Calculate(const clang::FunctionDecl* FD,
                              const DiffRequest& request);
    /// \brief Register variables to be used while accumulating error.
    /// \n Register variable declarations so that they may be used while
    /// calculating the final error estimates. Any unregistered variables will
    /// not be considered for the final estimation.
    /// \param[in] VD The variable declaration to be registered.
    /// \returns The Variable declaration of the '_delta_' prefixed variable.
    bool RegisterVariable(clang::VarDecl* VD);
    /// \brief Calculate aggregate error from m_EstimateVar.
    /// Builds the final error estimation statement.
    clang::Stmt* CalculateAggregateError();
    /// \brief Get the underlying DeclRefExpr type it it exists.
    /// \param[in] expr The expression whose DeclRefExpr is requested.
    /// \returns The DeclRefExpr of input or null.
    clang::DeclRefExpr* GetUnderlyingDeclRefOrNull(clang::Expr* expr);
    /// Get the parameter replacement (if any).
    /// \param[in] VD The parameter variable declaration to get replacement
    /// for.
    /// \returns The underlying replaced Expr.
    clang::Expr* GetParamReplacement(const clang::VarDecl* VD);

    friend class ReverseModeVisitor;
  };

} // namespace clad

#endif // CLAD_ERROR_ESTIMATOR_H