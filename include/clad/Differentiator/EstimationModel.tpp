#ifndef CLAD_ESTIMATION_MODEL_TPP
#define CLAD_ESTIMATION_MODEL_TPP

#ifndef CLAD_ESTIMATION_MODEL_H
#error __FILE__ should only be included from EstimationModel.h.
#endif

#include "clad/Differentiator/DerivativeBuilder.h"

#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/AST/OperationKinds.h"

using namespace clang;

namespace clad {

template <class T> Expr* EstimationModel<T>::IsVariableRegistered(VarDecl* VD) {
  auto it = m_EstimateVar.find(VD);
  if (it != m_EstimateVar.end())
    return it->second;
  return nullptr;
}

template <class T> VarDecl* EstimationModel<T>::AddVarToEstimate(VarDecl* VD) {
  auto deltaExpr = IsVariableRegistered(VD);
  if (!deltaExpr) {
    // If the varibale declaration is not registered, build another declaration
    // of the same type with '_delta_' prefix
    auto deltaDecl =
        m_VBase.BuildVarDecl(VD->getType(), "_delta_" + VD->getNameAsString());
    // Add it to the map that tracks the variables and their errors
    m_EstimateVar[VD] = m_VBase.BuildDeclRef(deltaDecl);
    return deltaDecl;
  }
  // If the varibale declaration was already registered, return the declaration
  // of the error it maps to
  return cast<VarDecl>(cast<DeclRefExpr>(deltaExpr)->getDecl());
}

template <class T> Stmt* EstimationModel<T>::CalculateAggregateError() {
  Expr* addExpr = nullptr;
  // Loop over all the error variables and form the final error expression of
  // the form... _final_error = _d_var * _delta_var + _d_var1 * _delta_var1 +...
  for (auto var : m_EstimateVar) {
    if (!addExpr) {
      addExpr =
          m_VBase.BuildOp(BO_Mul, m_VBase.m_Variables[var.first], var.second);
      continue;
    }
    // Form the espression '_d_vari * _delta_vari', here m_Variable maps each
    // variable declaration to its derivative
    auto currExpr =
        m_VBase.BuildOp(BO_Mul, m_VBase.m_Variables[var.first], var.second);
    // Now, accumulate the last expression into addExp, basically form something
    // like, addExpr = addExpr + _d_vari * _delta_vari
    addExpr = m_VBase.BuildOp(BO_Add, addExpr, currExpr);
  }
  // Build the final error declaration which is initalized with the addExpr we
  // formed before, basically
  //<Type> _final_error = _d_var * _delta_var + _d_var1 * _delta_var1 + ...
  // Where <Type> is the type of the final addition expression
  // (rhs in the above equation)
  m_AggregateEstimate =
      m_VBase.BuildVarDecl(addExpr->getType(), "_final_error", addExpr);
  // Return a declaration statement that can be directly emitted into the code
  // to be generated
  return m_VBase.BuildDeclStmt(m_AggregateEstimate);
}

/// Example class for taylor series approximation based error estimation
class TaylorApprox : public EstimationModel<TaylorApprox> {
public:
  // Return an expression of the following kind
  //  dfdx * delta_x
  Expr* AssignError(StmtDiff* refExpr, Expr* errExpr) {
    return m_VBase.BuildOp(BO_Mul, refExpr->getExpr_dx(), errExpr);
  }

  // For now, we can just return null
  // FIXME: return the true expression
  Expr* SetError(StmtDiff* declStmt) { return nullptr; }
};

} // namespace clad

#endif // CLAD_ESTIMATION_MODEL_TPP