// #include "visitors.hpp"
//
// #include "common/error.hpp"
// #include "common/expression.hpp"
// #include "common/token.hpp"
// #include "printer.hpp"
// #include "generator.hpp"
// #include "analyzer.hpp"
//
// #include <iostream>
// #include <string>
// #include <vector>
//
// std::vector<LocalVar> StmtAnalyzer::m_vars = {};
// int StmtAnalyzer::m_scope_depth = 0;
//
// void StmtAnalyzer::analyze(Stmt &stmt) { stmt.accept(*this); }
//
// void StmtAnalyzer::visit_print_stmt(PrintStmt &stmt)
// {
//     ExprAnalyzer analyzer(m_vars, m_scope_depth);
//     analyzer.analyze(*stmt.expr);
// }
//
// void StmtAnalyzer::visit_expr_stmt(ExprStmt &stmt)
// {
//     ExprAnalyzer analyzer(m_vars, m_scope_depth);
//     analyzer.analyze(*stmt.expr);
// }
//
// void StmtAnalyzer::visit_var_stmt(VarStmt &stmt)
// {
//     for (int ix = m_vars.size() - 1; ix >= 0; --ix)
//     {
//         if (stmt.var.token.lexeme == m_vars[ix].token.lexeme &&
//             m_vars[ix].scope_depth == m_scope_depth)
//         {
//             error::report(stmt.var.token.line, "Already defined variable");
//         }
//     }
//     m_vars.push_back(stmt.var);
//     if (m_vars.size() > 256)
//     {
//         error::report(
//             stmt.var.token.line, "Limit of 256 local vars has been exceeded"
//         );
//     }
//     stmt.var.rbp_offset = m_vars.size() * 8;
// }
//
// void StmtAnalyzer::visit_block_stmt(BlockStmt &stmt)
// {
//     m_scope_depth++;
//
//     StmtAnalyzer analyzer;
//     for (auto &stmt : stmt.statements)
//     {
//         analyzer.analyze(*stmt);
//     }
//
//     while (!m_vars.empty() && m_vars.back().scope_depth >= m_scope_depth)
//     {
//         m_vars.pop_back();
//     }
//
//     m_scope_depth--;
// }
//
// void StmtAnalyzer::visit_if_stmt(IfStmt &stmt)
// {
//     ExprAnalyzer expr_analyzer(m_vars, m_scope_depth);
//     expr_analyzer.analyze(*stmt.condition);
//
//     StmtAnalyzer analyzer;
//     analyzer.analyze(*stmt.then_branch);
//     analyzer.analyze(*stmt.else_branch);
// }
//
// void StmtAnalyzer::visit_while_stmt(WhileStmt &stmt)
// {
//     ExprAnalyzer expr_analyzer(m_vars, m_scope_depth);
//     expr_analyzer.analyze(*stmt.condition);
//
//     StmtAnalyzer stmt_analyzer;
//     stmt_analyzer.analyze(*stmt.body);
// }
//
// int StmtCodeGenerator::m_label_count = 0;
//
// std::string StmtCodeGenerator::generate(Stmt &stmt)
// {
//     m_output.clear();
//     m_output.str("");
//     stmt.accept(*this);
//     return m_output.str();
// }
//
// void StmtCodeGenerator::visit_print_stmt(PrintStmt &stmt)
// {
//     ExprCodeGenerator expr_generator;
//     m_output << expr_generator.generate(*stmt.expr);
//     m_output << "\tmov rdi, fmt ; 1st argument (format string)\n";
//     m_output << "\tpop rsi ; 2nd argument (integer to print)\n";
//     m_output << "\txor eax, eax ; Clear RAX: required before calling variadic "
//                 "functions like printf\n";
//     m_output << "\tcall printf\n";
// }
//
// void StmtCodeGenerator::visit_expr_stmt(ExprStmt &stmt)
// {
//     ExprCodeGenerator expr_generator;
//     m_output << expr_generator.generate(*stmt.expr);
// }
//
// void StmtCodeGenerator::visit_var_stmt(VarStmt &stmt)
// {
//     ExprCodeGenerator expr_generator;
//     m_output << expr_generator.generate(*stmt.expr);
//     m_output << "\tpop rax\n";
//     m_output << "\tmov qword [rbp - " << stmt.var.rbp_offset << "], rax\n";
// }
//
// void StmtCodeGenerator::visit_block_stmt(BlockStmt &stmt)
// {
//     StmtCodeGenerator generator;
//     for (auto const &stmt : stmt.statements)
//     {
//         m_output << generator.generate(*stmt);
//     }
// }
//
// void StmtCodeGenerator::visit_if_stmt(IfStmt &stmt)
// {
//     int else_label = m_label_count++;
//     int end_label = m_label_count++;
//
//     ExprCodeGenerator expr_generator;
//     m_output << expr_generator.generate(*stmt.condition);
//
//     m_output << "\tpop rax\n";
//     m_output << "\tcmp rax, 0\n";
//     m_output << "\tje L" << else_label << "\n";
//
//     StmtCodeGenerator stmt_generator;
//     m_output << stmt_generator.generate(*stmt.then_branch);
//     m_output << "\tjmp L" << end_label << "\n";
//
//     m_output << "L" << else_label << ":\n";
//     if (stmt.else_branch != nullptr)
//     {
//         m_output << stmt_generator.generate(*stmt.else_branch);
//     }
//
//     m_output << "L" << end_label << ":\n";
// }
//
// void StmtCodeGenerator::visit_while_stmt(WhileStmt &stmt)
// {
//     int start_label = m_label_count++;
//     int end_label = m_label_count++;
//
//     m_output << "L" << start_label << ":\n";
//
//     ExprCodeGenerator expr_generator;
//     m_output << expr_generator.generate(*stmt.condition);
//
//     m_output << "\tpop rax\n";
//     m_output << "\tcmp rax, 0\n";
//     m_output << "\tje L" << end_label << "\n";
//
//     StmtCodeGenerator body_generator;
//     m_output << body_generator.generate(*stmt.body);
//
//     m_output << "\tjmp L" << start_label << "\n";
//
//     m_output << "L" << end_label << ":\n";
// }
