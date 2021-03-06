#ifdef _cplusplus
extern "C" {
#endif
#include "probal.h"


/* Function:  write_probabilistic_models(dfp,gm,dpi)
 *
 * Descrip:    Makes all the probabilistic routines
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:         gm [UNKN ] Undocumented argument [GenericMatrix *]
 * Arg:        dpi [UNKN ] Undocumented argument [DPImplementation *]
 *
 */
# line 17 "probal.dy"
void write_probabilistic_models(DYNFILE * dfp,GenericMatrix * gm,DPImplementation * dpi)
{
  write_score_only_sum(dfp,gm,dpi);

  write_expl_mat_sum(dfp,gm,dpi);
}

/* Function:  write_expl_mat_sum(dfp,gm,dpi)
 *
 * Descrip:    Makes the explicit matrix over all paths
 *             construction method
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:         gm [UNKN ] Undocumented argument [GenericMatrix *]
 * Arg:        dpi [UNKN ] Undocumented argument [DPImplementation *]
 *
 */
# line 28 "probal.dy"
void write_expl_mat_sum(DYNFILE * dfp,GenericMatrix * gm,DPImplementation * dpi)
{
  FuncInfo * fi;
  char * arg_str;
  char * chainstr;
  
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"matrix_logsum_%s",gm->name);
  add_line_to_Ftext(fi->ft,"This function calculates the matrix over all paths");
  add_line_to_Ftext(fi->ft,"This is using a logsum method to sort it all out");

  arg_str = get_argstr_GenericMatrix(gm);
  chainstr = get_chainstr_GenericMatrix(gm);
  add_args_GenericMatrix_FuncInfo(fi,gm);

  start_function_FuncInfo(fi,dfp,"%s * matrix_logsum_%s(%s)",gm->name,gm->name,arg_str);
  ckfree(arg_str);

  expr(dfp,"%s * mat;",gm->name);
  expr(dfp,"int i;");
  expr(dfp,"int j;");
  expr(dfp,"int leni;");
  expr(dfp,"int lenj;");
  expr(dfp,"int tot;");
  expr(dfp,"int num;");
  add_break(dfp);

  expr(dfp,"if( (mat=allocate_Expl_%s(%s)) == NULL )",gm->name,chainstr);
  startbrace(dfp);
  warn_expr(dfp,"Unable to allocate large %s version",gm->name);
  expr(dfp,"return NULL;");
  closebrace(dfp);

  expr(dfp,"leni = mat->leni;");
  expr(dfp,"lenj = mat->lenj;");

  
  /*** make num and tot ***/
  
  expr(dfp,"tot = leni * lenj;");
  expr(dfp,"num = 0;");
  

  /*** see if there any specials to specials to do ***/
  
  add_break(dfp);
  expr(dfp,"start_reporting(\"%s Matrix calculation: \");",gm->name);
  
  expr(dfp,"for(j=0;j<lenj;j++)");
  startbrace(dfp);
  expr(dfp,"auto int score");
  expr(dfp,"auto int temp");
  expr(dfp,"for(i=0;i<leni;i++)");
  startbrace(dfp);

  expr(dfp,"if( num%%1000 == 0 )");
  hang_expr(dfp,"log_full_error(REPORT,0,\"[%%7d] Cells %%2d%%%%%%%%\",num,num*100/tot);");
  

/**  expr(dfp,"(void) do_cell_%s(mat,i,j)",gm->name); **/

  expr(dfp,"num++;"); 

  gm->calcfunc = dpi->calcfunc;

  write_score_block(dfp,gm,"EXPL_MATRIX","mat","EXPL_SPECIAL",TRUE);


  closebrace(dfp);

  write_special_block(dfp,gm,"EXPL_MATRIX","EXPL_SPECIAL",NULL);

  gm->calcfunc = NULL;
  closebrace(dfp);
  
  /*** stop reporting ***/
  
  expr(dfp,"stop_reporting()");
    
  expr(dfp,"return mat");
  
  close_function(dfp);
  
  add_break(dfp);
}  

/* Function:  write_score_only_sum(dfp,gm,dpi)
 *
 * Descrip:    Makes the single one-on-one over all paths
 *             searching method
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:         gm [UNKN ] Undocumented argument [GenericMatrix *]
 * Arg:        dpi [UNKN ] Undocumented argument [DPImplementation *]
 *
 */
# line 117 "probal.dy"
void write_score_only_sum(DYNFILE * dfp,GenericMatrix * gm,DPImplementation * dpi)
{
  int i;
  FuncInfo * fi;
  char * arg_str;
  char * chain_str;
  
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"score_only_logsum_%s",gm->name);
  add_line_to_Ftext(fi->ft,"This function calculates the score over all paths");
  add_line_to_Ftext(fi->ft,"This is using a logsum method to sort it all out");

  arg_str = get_argstr_GenericMatrix(gm);
  add_args_GenericMatrix_FuncInfo(fi,gm);

  start_function_FuncInfo(fi,dfp,"Score score_only_logsum_%s(%s)",gm->name,arg_str);
  ckfree(arg_str);

  expr(dfp,"int i;");
  expr(dfp,"int j;");
  expr(dfp,"int bestscore = 0;");
  expr(dfp,"int k;");
  expr(dfp,"%s * mat",gm->name);


  if(gm->qtype != NULL && gm->qtype->maxlen != NULL) {
    expr(dfp,"int internal_matrix[%d][(%s+%d) * %d];",gm->window_j+1,gm->qtype->maxlen,gm->window_i,gm->len);
    expr(dfp,"int internal_specials[%d][%d];",gm->window_j+1,gm->spec_len);
  }

  
  add_break(dfp);
  chain_str = get_chainstr_GenericMatrix(gm);
  expr(dfp,"mat = allocate_%s_only(%s);",gm->name,chain_str);
  ckfree(chain_str);

  expr(dfp,"if( mat == NULL )");
  startbrace(dfp);
  warn_expr(dfp,"Memory allocation error in the db search - unable to communicate to calling function. this spells DISASTER!");
  expr(dfp,"return NEGI");
  closebrace(dfp);

  if(gm->qtype != NULL && gm->qtype->maxlen != NULL) {
    add_block_comment(dfp,"Ok,don't need to allocate matrix as it is internal, because we have a max length");
  } else {
    expr(dfp,"if((mat->basematrix = BaseMatrix_alloc_matrix_and_specials(%d,(mat->leni + %d) * %d,%d,%d)) == NULL)",gm->window_j+1,gm->window_i,gm->len,gm->window_j+1,gm->spec_len);
    startbrace(dfp);
    expr(dfp,"warn(\"Score only matrix for %s cannot be allocated, (asking for %d  by %%d  cells)\",mat->leni*%d);",gm->name,gm->window_j,gm->len);
    expr(dfp,"mat = free_%s(mat)",gm->name);
    expr(dfp,"return 0;");
    closebrace(dfp);
    expr(dfp,"mat->basematrix->type = BASEMATRIX_TYPE_VERYSMALL;");
  }

  add_break(dfp);

  add_block_comment(dfp,"Now, initiate matrix");

  expr(dfp,"for(j=0;j<%d;j++)",gm->window_j+2);
  startbrace(dfp);
  expr(dfp,"for(i=(-%d);i<mat->leni;i++)",gm->window_i);
  startbrace(dfp);
  expr(dfp,"for(k=0;k<%d;k++)",gm->len);
  hang_expr(dfp,"%s_VSMALL_MATRIX(mat,i,j,k) = NEGI;",gm->name);
  closebrace(dfp);
  for(i=0;i<gm->spec_len;i++) {
    expr(dfp,"%s_VSMALL_SPECIAL(mat,i,j,%s) = %s;",gm->name,gm->special[i]->name,gm->special[i]->def_score);
  }
  closebrace(dfp);

  add_break(dfp);

  add_block_comment(dfp,"Ok, lets do-o-o-o-o it");

  add_break(dfp);

  expr(dfp,"for(j=0;j<mat->lenj;j++)");
  startbrace_tag(dfp,"for all target positions");
  expr(dfp,"auto int score");
  expr(dfp,"auto int temp");
  expr(dfp,"for(i=0;i<mat->leni;i++)");
  startbrace_tag(dfp,"for all query positions");
  
  gm->calcfunc = dpi->calcfunc;
  write_score_block(dfp,gm,"VSMALL_MATRIX","mat","VSMALL_SPECIAL",TRUE);
  gm->calcfunc = NULL;

  closebrace(dfp);

  add_break(dfp);

  gm->calcfunc = dpi->calcfunc;
  write_special_block(dfp,gm,"VSMALL_MATRIX","VSMALL_SPECIAL",NULL);
  gm->calcfunc = NULL;

  closebrace(dfp);

  add_break(dfp);

  expr(dfp,"mat = free_%s(mat)",gm->name);

  expr(dfp,"return bestscore;");

  close_function(dfp);

  add_break(dfp);
}






# line 239 "probal.c"

#ifdef _cplusplus
}
#endif
