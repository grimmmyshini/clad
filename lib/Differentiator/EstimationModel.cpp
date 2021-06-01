#include "clad/Differentiator/DerivativeBuilder.h"
#include "clad/Differentiator/EstimationModel.h"

#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/AST/OperationKinds.h"

using namespace clang;

namespace clad {

EstimationModel::EstimationModel() {}
EstimationModel::~EstimationModel() {}

Expr* EstimationModel::IsVariableRegistered(const VarDecl* VD) {
  auto it = m_EstimateVar.find(VD);
  if (it != m_EstimateVar.end())
    return it->second;
  return nullptr;
}

VarDecl* EstimationModel::AddVarToEstimate(VarDecl* VD) {
  auto deltaExpr = IsVariableRegistered(VD);
  if (!deltaExpr) {
    // Get the initital error (if any)
    auto initExpr = SetError(VD);
    if (!initExpr)
      initExpr = m_VBase->getZeroInit(m_VBase->m_Context.DoubleTy);
    // If the varibale declaration is not registered, build another declaration
    // of the same type with '_delta_' prefix
    auto deltaDecl =
        m_VBase->BuildVarDecl(m_VBase->m_Context.DoubleTy,
                              "_delta_" + VD->getNameAsString(), initExpr);
    // Add it to the map that tracks the variables and their errors
    m_EstimateVar.emplace(VD, m_VBase->BuildDeclRef(deltaDecl));
    return deltaDecl;
  }
  // If the varibale declaration was already registered, return the declaration
  // of the error it maps to
  return cast<VarDecl>(cast<DeclRefExpr>(deltaExpr)->getDecl());
}

Expr* EstimationModel::CalculateAggregateError() {
  Expr* addExpr = nullptr;
  // Loop over all the error variables and form the final error expression of
  // the form... _final_error = _delta_var + _delta_var1 +...
  for (auto var : m_EstimateVar) {
    if (!addExpr) {
      addExpr = var.second;
      continue;
    }
    addExpr = m_VBase->BuildOp(BO_Add, addExpr, var.second);
  }
  // Return an expression that can be directly assigned to final error
  return addExpr;
}

Expr* EstimationModel::GetFloatingTypeLiteral(double val){
  return FloatingLiteral::Create(m_VBase->m_Context, llvm::APFloat(val), true,
                                 m_VBase->m_Context.DoubleTy, noLoc);
}

Expr* TaylorApprox::AssignError(StmtDiff refExpr, Expr* errExpr) {
    auto epsExpr =
        GetFloatingTypeLiteral(std::numeric_limits<float>::epsilon());
    return m_VBase->BuildOp(BO_Mul, refExpr.getExpr_dx(),
                           m_VBase->BuildOp(BO_Mul, refExpr.getExpr(), epsExpr));
}

Expr* TaylorApprox::SetError(VarDecl* declStmt) { return nullptr; }

} // namespace clad
