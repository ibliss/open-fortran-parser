#include <stdio.h>
#include "OFPTokenSource.h"
#include "CFortranParser.h"
#include "FTreeWalker.h"

pANTLR3_VECTOR   get_tokens(const char * token_file);

int main(int argc, char * argv[])
{
   int i;

   pANTLR3_VECTOR                 tlist;
   pANTLR3_TOKEN_SOURCE           tsource;
   pANTLR3_COMMON_TOKEN_STREAM    tstream;
   pCFortranParser                parser;

   CFortranParser_main_program_return main_ast;
   
   char * tok_file = argv[1];
   char * src_file = argv[2];

   if (argc < 3) {
      printf("usage: ftokens token_file src_file\n");
      exit(1);
   }

   /* Lexer phase
    *    - Call the token parser to read the tokens from the token file.
    */

   tlist = get_tokens(tok_file);

   /* print tokens
    */
   //   for (i = 0; i < tlist->size(tlist); i++) {
   //      print_token((pANTLR3_COMMON_TOKEN) tlist->get(tlist, i));
   //   }

   /* Parser phase
    *    - Call the parser with the token source which uses the token
    *      list obtained from the token file.
    */

   tsource  =  ofpTokenSourceNew                 ( (pANTLR3_UINT8) src_file, tlist );
   tstream  =  antlr3CommonTokenStreamSourceNew  ( ANTLR3_SIZE_HINT, tsource );
   parser   = CFortranParserNew                  ( tstream );

   main_ast = parser->main_program(parser);

   if (parser->pParser->rec->state->errorCount > 0)
   {
      fprintf(stderr, "The parser returned %d errors, tree walking aborted.\n", parser->pParser->rec->state->errorCount);
   }
   else
   {
      pANTLR3_COMMON_TREE_NODE_STREAM nodes;
      pFTreeWalker tree_parser;

      printf("Tree : %s\n", main_ast.tree->toStringTree(main_ast.tree)->chars);

      nodes = antlr3CommonTreeNodeStreamNewTree(main_ast.tree, ANTLR3_SIZE_HINT);

      tree_parser = FTreeWalkerNew(nodes);
      tree_parser->main_program(tree_parser);

      nodes       ->free(nodes);          nodes       = NULL;
      tree_parser ->free(tree_parser);    tree_parser = NULL;
   }

   parser  ->free(parser);                parser      = NULL;
   tstream ->free(tstream);               tstream     = NULL;
   // TODO tsource ->free(tsource);               tsource     = NULL;

   return 0;
}

void printToken(pANTLR3_COMMON_TOKEN tok)
{
   printf("[");
     printf("@\%d,", (int)tok->getTokenIndex(tok));
     printf("\%d:", (int)tok->getStartIndex(tok));
     printf("\%d=", (int)tok->getStopIndex(tok));
     printf("'\%s',", tok->getText(tok)->chars);
     printf("<\%d>,", (int)tok->getType(tok));
     if (tok->getChannel(tok) > ANTLR3_TOKEN_DEFAULT_CHANNEL) {
        printf("channel=\%d,", (int)tok->getChannel(tok));
     }
     printf("\%d:", (int)tok->getLine(tok));
     printf("\%d", (int)tok->getCharPositionInLine(tok));
   printf("]\n");
}
