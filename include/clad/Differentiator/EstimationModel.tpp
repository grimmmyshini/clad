#ifndef CLAD_ESTIMATION_MODEL_TPP
#define CLAD_ESTIMATION_MODEL_TPP

#ifndef CLAD_ESTIMATION_MODEL_H
#error __FILE__ should only be included from EstimationModel.h.
#endif

#include "DerivativeBuilder.h"

#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/AST/OperationKinds.h"

using namespace clang;

namespace clad {

template <class T>
Expr* EstimationModel<T>::AssignError(StmtDiff refExpr, clang::Expr* errExpr) {
  return static_cast<T*>(this)->AssignError(refExpr, errExpr);
}

template <class T> Expr* EstimationModel<T>::SetError(VarDeclDiff declStmt) {
  return static_cast<T*>(this)->SetError(declStmt);
}

template <class T> Expr* EstimationModel<T>::IsVariableRegistered(const VarDecl* VD) {
  auto it = m_EstimateVar.find(VD);
  if (it != m_EstimateVar.end())
    return it->second;
  return nullptr;
}

template <class T>
VarDecl* EstimationModel<T>::AddVarToEstimate(const VarDecl* VD) {
  auto deltaExpr = IsVariableRegistered(VD);
  if (!deltaExpr) {
    // If the varibale declaration is not registered, build another declaration
    // of the same type with '_delta_' prefix
    auto deltaDecl = m_VBase.BuildVarDecl(
        m_VBase.m_Context.DoubleTy, "_delta_" + VD->getNameAsString(),
        m_VBase.getZeroInit(m_VBase.m_Context.DoubleTy));
    // Add it to the map that tracks the variables and their errors
    m_EstimateVar.emplace(VD, m_VBase.BuildDeclRef(deltaDecl));
    return deltaDecl;
  }
  // If the varibale declaration was already registered, return the declaration
  // of the error it maps to
  return cast<VarDecl>(cast<DeclRefExpr>(deltaExpr)->getDecl());
}

template <class T> Expr* EstimationModel<T>::CalculateAggregateError() {
  Expr* addExpr = nullptr;
  // Loop over all the error variables and form the final error expression of
  // the form... _final_error = _delta_var + _delta_var1 +...
  for (auto var : m_EstimateVar) {
    if (!addExpr) {
      addExpr = var.second;
      continue;
    }
    addExpr = m_VBase.BuildOp(BO_Add, addExpr, var.second);
  }
  // Return an expression that can be directly assigned to final error
  return addExpr;
}

template <class T> Expr* EstimationModel<T>::GetFloatingTypeLiteral(double val){
  return FloatingLiteral::Create(m_VBase.m_Context, llvm::APFloat(val), true,
                                 m_VBase.m_Context.DoubleTy, noLoc);
}

/// Example class for taylor series approximation based error estimation
class TaylorApprox : public EstimationModel<TaylorApprox> {
public:
  // Return an expression of the following kind
  //  dfdx * delta_x
  Expr* AssignError(StmtDiff refExpr, Expr* errExpr = nullptr) {
    auto epsExpr =
        GetFloatingTypeLiteral(std::numeric_limits<float>::epsilon());
    return m_VBase.BuildOp(BO_Mul, refExpr.getExpr_dx(),
                           m_VBase.BuildOp(BO_Mul, refExpr.getExpr(), epsExpr));
  }

  // For now, we can just return null
  // FIXME: return the true expression
  Expr* SetError(VarDeclDiff declStmt) { return nullptr; }
};

} // namespace clad

#endif // CLAD_ESTIMATION_MODEL_TPP