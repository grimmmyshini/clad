#ifndef CLAD_ESTIMATION_MODEL_H
#define CLAD_ESTIMATION_MODEL_H

#include <unordered_map>

namespace clang {
class VarDecl;
class Stmt;
class Expr;
} // namespace clang

namespace clad {
class StmtDiff;
class VisitorBase;
class ReverseModeVisitor;
class VarDeclDiff;
} // namespace clad

namespace clad {

/// A class to facilitate usage of user defined error estimation models
class EstimationModel {
protected:
  /// An aggregate error estimate taking into account all intermediate
  /// floats/truncated int values
  clang::VarDecl* m_AggregateEstimate;
  /// Map to keep track of the error estimate variables for each declaration
  /// reference could be removed if we come up with annotations for the estimate
  /// vars
  std::unordered_map<const clang::VarDecl*, clang::Expr*> m_EstimateVar;
  /// This is the reference to the VisitorBase class which will be used to build
  /// various kinds of expressions and statements
  VisitorBase* m_VBase = nullptr;

public:
  EstimationModel();
  virtual ~EstimationModel();
  /// \brief Check if a variable is registered for estimation
  /// \param[in] VD The variable to check
  /// \returns The delta expression of the variable if it is registered, nullptr
  /// otherwise
  clang::Expr* IsVariableRegistered(const clang::VarDecl* VD);
  /// \brief Track the varibale declaration and utilize it in error estimation
  /// \param[in] VD The declaration to track
  /// \returns The varibale declaration of the delta value
  clang::VarDecl* AddVarToEstimate(clang::VarDecl* VD);
  /// \brief User overridden function to return the error expression of a
  /// specific estimation model. The error expression is returned in the form of
  /// a clang::Expr, the user may use BuildOp() to build the final expression.
  /// An example of a possible override is:
  /// \code
  /// clang::Expr* AssignError(clad::StmtDiff* refExpr, clang::Expr* errExpr) {
  ///   return BuildOp(BO_Mul, refExpr->getExpr_dx(), errExpr);
  /// }
  /// \endcode
  /// The above returns the expression: drefExpr * errExpr which is the error
  /// expression for Taylor approximation model
  ///
  /// \param[in] refExpr The reference of the expression to which the error has
  /// to be assigned, this is a StmtDiff type hence one can use getExpr() to
  /// get the unmodified expression and getExpr_dx() to get the absolute
  /// derivative of the same
  /// \param[in] errExpr This is the error in the refExpr so far. Errors are
  /// assigned to expressions at every step so this value varies as does the
  /// depth of the main expression we are evaluating.
  virtual clang::Expr *AssignError(StmtDiff refExpr, clang::Expr *errExpr = nullptr) = 0;
  /// \brief Initializes errors for '_delta_' statements
  /// This function returns the initial error assignment. Similar to
  /// AssignError, however, this function is only called during declaration of
  /// variables. This function is separate from AssignError to keep
  /// implementation of different estimation models more flexible.
  ///
  /// Following the example above, a possible override is:
  /// \code
  /// clang::Expr* SetError(clang::VarDecl* declStmt) {
  ///      return nullptr;
  /// }
  /// \endcode
  /// The above will return a 0 expression to be assigned to the '_delta_'
  /// declaration of decl
  ///
  /// \param[in] decl The declaration to which the error has to be assigned.
  /// \returns The error expression for declaration statements.
  virtual clang::Expr *SetError(clang::VarDecl* decl) = 0;
  /// \brief Calculate aggregate error from m_EstimateVar.
  /// Builds the final error estimation statement
  clang::Expr* CalculateAggregateError();
  /// \brief Gets a floating point value as a literal expr
  /// \param[in] val The value to turn into a expr
  /// \returns A literal expression equivalent to the input val
  clang::Expr* GetFloatingTypeLiteral(double val);

  friend class ErrorEstimationHandler;
};

/// Example class for taylor series approximation based error estimation
class TaylorApprox : public EstimationModel{
public:
  // Return an expression of the following kind
  //  dfdx * delta_x
  clang::Expr *AssignError(StmtDiff refExpr, clang::Expr *errExpr = nullptr);

  // For now, we can just return null
  clang::Expr *SetError(clang::VarDecl* decl);
};

} // namespace clad

#endif // CLAD_ESTIMATION_MODEL_H