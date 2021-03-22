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
} // namespace clad

namespace clad {

/// A class to facilitate usage of user defined error estimation models
/// \tparam SubModel The user derived sub class which implements a custom
/// estimation model
template <class SubModel> class EstimationModel {
protected:
  /// An aggregate error estimate taking into account all intermediate
  /// floats/truncated int values
  clang::VarDecl* m_AggregateEstimate;
  /// Map to keep track of the error estimate variables for each declaration
  /// reference could be removed if we come up with annotations for the estimate
  /// vars
  std::unordered_map<clang::VarDecl*, clang::Expr*> m_EstimateVar;
  /// This is the reference to the VisitorBase class which will be used to build
  /// various kinds of expressions and statements
  clad::VisitorBase& m_VBase;

public:
  EstimationModel();
  ~EstimationModel();
  /// \brief Set the visitor base instance that will help us later build
  /// operations. Internally called by Calculate
  /// \param[in] vBase The instance to set
  void SetVisitorBase(clad::VisitorBase& vBase) { m_VBase = vBase; }
  /// \brief Check if a variable is registered for estimation
  /// \param[in] VD The variable to check
  /// \returns The delta expression of the variable if it is registered, nullptr otherwise
  clang::Expr* IsVariableRegistered(clang::VarDecl* VD);
  /// \brief Track the varibale declaration and utilize it in error estimation
  /// \param[in] VD The declaration to track
  /// \returns The varibale declaration of the delta value
  clang::VarDecl* AddVarToEstimate(clang::VarDecl* VD);
  /// \brief User overridden function to return the error expression of a
  /// specific estimation model. The error expression is returned in the form of
  /// a clang::Expr, the user may use BuildOp() to build the final expression.
  /// An example of a possible override is:
  /// \code
  /// clang::Expr* AssignError(const clad::StmtDiff* refExpr, const clang::Expr*
  /// errExpr) {
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
  clang::Expr* AssignError(const clad::StmtDiff* refExpr,
                           const clang::Expr* errExpr) {
    return static_cast<SubModel*>(this)->AssignError(refExpr, errExpr);
  }
  /// \brief Assign errors for declaration statements.
  /// This function returns the initial error assignment. Similar to
  /// AssignError, however, this function is only called during declaration of
  /// variables. This function is separate from AssignError to keep
  /// implementation of different estimation models more flexible.
  ///
  /// Following the example above, a possible override is:
  /// \code
  /// clang::Expr* SetError(const clad::StmtDiff* declStmt) {
  ///      return BuildOp(BO_Mul, declStmt->getExpr_dx(), DBL_EPSILON);
  /// }
  /// \endcode
  /// The above returns an expression: declStmt * Em
  ///
  /// \param[in] declStmt The declaration to which the error has to be assigned.
  /// \returns The error expression for declaration statements.
  clang::Expr* SetError(const clad::StmtDiff* declStmt) {
    return static_cast<SubModel*>(this)->SetError(declStmt);
  }
  /// \brief Calculate aggregate error from m_EstimateVar.
  /// Builds the final error estimation statement
  clang::Stmt* CalculateAggregateError();
};

} // namespace clad

#include "EstimationModel.tpp"

#endif // CLAD_ESTIMATION_MODEL_H