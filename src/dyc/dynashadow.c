#ifdef _cplusplus
extern "C" {
#endif
#include "dynashadow.h"
#include "dynafunc.h"


# line 19 "dynashadow.dy"
void write_dc_functions(DYNFILE * dfp,GenericMatrix * gm)
{

  write_shadow_dc_macros(dfp,gm);

  alloc_small_func_GenericMatrix(dfp,gm);

  make_small_calculate_func(dfp,gm);

  one_shot_AlnRangeSet_func(dfp,gm);

  write_AlnRangeSet_build_func(dfp,gm);

  write_AlnRange_build_func(dfp,gm);


  /*  write_dc_PackAln_build_func(dfp,gm); */

  write_hidden_read_func(dfp,gm);

  write_hidden_max_func(dfp,gm);

  write_special_strip_read_func(dfp,gm);

  write_special_strip_max_func(dfp,gm);

  write_matrix_to_special_max_func(dfp,gm);

  write_hidden_calc_func(dfp,gm);
  
  write_hidden_init_func(dfp,gm);

  write_full_dc_func(dfp,gm);

  write_single_dc_pass_func(dfp,gm);

  write_push_dc_func(dfp,gm);

  write_follow_on_dc_func(dfp,gm);

  write_up_to_dc_func(dfp,gm);

  write_init_dc_func(dfp,gm);

  /*   write_shadow_dc_alloc(dfp,gm); */

/*  write_start_end_macros(dfp,gm); */

/*  write_shadow_start_end_alloc(dfp,gm); */

/*  write_start_end_build(dfp,gm);*/

  write_start_end_find_end(dfp,gm);

  optimised_shadow_GenericMatrix(dfp,gm);

/*  heavy_optimised_shadow_GenericMatrix(dfp,gm);*/

  write_start_end_init(dfp,gm);

}


/* Function:  alloc_small_func_GenericMatrix(dfp,gm)
 *
 * Descrip:    makes the allocate_Small_xxx function.
 *             This calls allocate_xxx_only function
 *             (made by /write_safe_alloc_function)
 *             and then allocates basematrix stuff as well.
 *
 *
 * Arg:        dfp [UNKN ] dynamite file pointer [DYNFILE *]
 * Arg:         gm [READ ] generic matrix structure [const GenericMatrix *]
 *
 */
# line 91 "dynashadow.dy"
void alloc_small_func_GenericMatrix(DYNFILE * dfp,const GenericMatrix * gm)
{
  char * arg_str;
  char * chain_str;
  FuncInfo * fi;
  fi = FuncInfo_named_from_varstr(FI_CALLABLE,"allocate_Small_%s",gm->name);


  /*** prepare function information ***/

  
  add_line_to_Ftext(fi->ft,"This function allocates the %s structure",gm->name);
  add_line_to_Ftext(fi->ft,"and the basematrix area for a small memory implementations");
  add_line_to_Ftext(fi->ft,"It calls /allocate_%s_only",gm->name);

  arg_str = get_argstr_GenericMatrix(gm);
  add_args_GenericMatrix_FuncInfo(fi,gm);


  start_function_FuncInfo(fi,dfp,"%s * allocate_Small_%s(%s)",gm->name,gm->name,arg_str);

  /*** clean up ***/
  ckfree(arg_str);



  /*** into function body ***/


  expr(dfp,"%s * out;",gm->name);
  
  add_break(dfp);
  chain_str = get_chainstr_GenericMatrix(gm);
  expr(dfp,"out = allocate_%s_only(%s);",gm->name,chain_str);
  ckfree(chain_str);

  expr(dfp,"if( out == NULL )");
  hang_expr(dfp,"return NULL;");

  expr(dfp,"out->basematrix = BaseMatrix_alloc_matrix_and_specials(%d,(out->leni + %d) * %d,%d,out->lenj+%d)",(gm->window_j+1)*8,gm->window_i,gm->len,gm->spec_len*8,gm->window_j);
  
  expr(dfp,"if(out == NULL) ");
  startbrace(dfp);
  expr(dfp,"warn(\"Small shadow matrix %s cannot be allocated, (asking for %d by %%d main cells)\",out->leni+%d);",gm->name,gm->window_j+1,gm->window_i+1);
  expr(dfp,"free_%s(out)",gm->name);
  expr(dfp,"return NULL;");
  closebrace(dfp);

  expr(dfp,"out->basematrix->type = BASEMATRIX_TYPE_SHADOW;");

  expr(dfp,"return out");
  
  close_function(dfp);
  
  add_break(dfp);
  
}

/* Function:  make_small_calculate_func(dfp,gm)
 *
 * Descrip:    make the calculate function for
 *             small PackAln system
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:         gm [UNKN ] Undocumented argument [GenericMatrix *]
 *
 */
# line 153 "dynashadow.dy"
void make_small_calculate_func(DYNFILE * dfp,GenericMatrix * gm)
{
  FuncInfo * fi;
  register int s;
  int ss;

  /*** yawn. There are better ways to do this! ***/

  for(s=0;s<gm->spec_len;s++)
    if( gm->special[s]->is_end == TRUE )
      break; 

  for(ss=0;ss<gm->spec_len;ss++) 
    if( gm->special[ss]->is_start == TRUE)
      break;


  
  fi = FuncInfo_named_from_varstr(FI_CALLABLE,"PackAln_calculate_Small_%s",gm->name);
  add_line_to_Ftext(fi->ft,"This function calculates an alignment for %s structure in linear space",gm->name);
  add_line_to_Ftext(fi->ft,"If you want only the start/end points");
  add_line_to_Ftext(fi->ft,"use /AlnRangeSet_calculate_Small_%s ",gm->name);
  add_break_to_Ftext(fi->ft);
  add_line_to_Ftext(fi->ft,"The function basically");
  add_line_to_Ftext(fi->ft,"  finds start/end points ");
  add_line_to_Ftext(fi->ft,"  foreach start/end point ");
  add_line_to_Ftext(fi->ft,"    calls /full_dc_%s ",gm->name);

  start_function_FuncInfo(fi,dfp,"PackAln * PackAln_calculate_Small_%s(%s * mat,DPEnvelope * dpenv)",gm->name,gm->name);

  /*** function body ***/

  expr(dfp,"int endj;");
  expr(dfp,"int score;");
  expr(dfp,"PackAln * out;");
  expr(dfp,"PackAlnUnit * pau;");
  expr(dfp,"int starti;");
  expr(dfp,"int startj;");
  expr(dfp,"int startstate;");
  expr(dfp,"int stopi;");
  expr(dfp,"int stopj;");
  expr(dfp,"int stopstate;");
  expr(dfp,"int temp;");
  expr(dfp,"int donej;");
  add_end_comment(dfp,"This is for reporting, will be passed as a & arg in");
  expr(dfp,"int totalj;");
  add_end_comment(dfp,"This also is for reporting, but as is not changed, can be passed by value");

  add_break(dfp);

  expr(dfp,"if( mat->basematrix->type != BASEMATRIX_TYPE_SHADOW )");
  startbrace(dfp);
  warn_expr(dfp,"Could not calculate packaln small for %s due to wrong type of matrix",gm->name);
  expr(dfp,"return NULL;");
  closebrace(dfp);

  add_break(dfp);

  expr(dfp,"out = PackAln_alloc_std();");


  add_break(dfp);
  
  /** got to figure out start/end first **/

  expr(dfp,"start_reporting(\"Find start end points: \");");
  expr(dfp,"dc_optimised_start_end_calc_%s(mat,dpenv);",gm->name);
  
  expr(dfp,"score = start_end_find_end_%s(mat,&endj);",gm->name);
  expr(dfp,"out->score = score;");
  expr(dfp,"stopstate = %s;\n",gm->special[s]->name);


  /*** ok, life is different if we have special to specials. ***/

  if( gm->specialtospecial == FALSE) {
    add_block_comment(dfp,"No special to specials: one matrix alignment: simply remove and get");
    expr(dfp,"starti = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,%s,0);",gm->name,gm->special[s]->name);
    expr(dfp,"startj = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,%s,1);",gm->name,gm->special[s]->name);
    expr(dfp,"startstate = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,%s,2);",gm->name,gm->special[s]->name);
    expr(dfp,"stopi = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,%s,3);",gm->name,gm->special[s]->name);
    expr(dfp,"stopj = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,%s,4);",gm->name,gm->special[s]->name);
    expr(dfp,"stopstate = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,%s,5);",gm->name,gm->special[s]->name);
    expr(dfp,"temp = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,%s,6);",gm->name,gm->special[s]->name);
    expr(dfp,"log_full_error(REPORT,0,\"[%%d,%%d][%%d,%%d] Score %%d\",starti,startj,stopi,stopj,score);");
    expr(dfp,"stop_reporting();");
    
    expr(dfp,"start_reporting(\"Recovering alignment: \");");
    
    add_break(dfp);
    add_block_comment(dfp,"Figuring how much j we have to align for reporting purposes");
    expr(dfp,"donej = 0;");
    expr(dfp,"totalj = stopj - startj;");
    
    expr(dfp,"full_dc_%s(mat,starti,startj,startstate,stopi,stopj,stopstate,out,&donej,totalj,dpenv);",gm->name);
    add_break(dfp);
    add_block_comment(dfp,"Although we have no specials, need to get start. Better to check than assume");
    add_break(dfp);

    expr(dfp,"max_matrix_to_special_%s(mat,starti,startj,startstate,temp,&stopi,&stopj,&stopstate,&temp,NULL);",gm->name);
    expr(dfp,"if( stopi == %s_READ_OFF_ERROR || stopstate != %s )",gm->name,gm->special[ss]->name);
    startbrace(dfp);
    warn_expr(dfp,"Problem in reading off special state system, hit a non start state (or an internal error) in a single alignment mode");
    expr(dfp,"invert_PackAln(out);");
    expr(dfp,"recalculate_PackAln_%s(out,mat);",gm->name);
    expr(dfp,"return out;");
    closebrace(dfp);

    add_break(dfp);
    
    /*** Need to add PackUnit alloc etc... ***/
    add_block_comment(dfp,"Ok. Put away start start...");
    expr(dfp,"pau = PackAlnUnit_alloc()");
    expr(dfp,"pau->i = stopi;");
    expr(dfp,"pau->j = stopj;");
    expr(dfp,"pau->state = stopstate + %d;",gm->len);
    expr(dfp,"add_PackAln(out,pau)");

    add_break(dfp);

    
    
    expr(dfp,"log_full_error(REPORT,0,\"Alignment recovered\");");
    expr(dfp,"stop_reporting();");
  }
  else {
    add_block_comment(dfp,"Special to specials: have to eat up in strip and then drop back to full_dc for intervening bits");
    expr(dfp,"log_full_error(REPORT,0,\"End at %%d Score %%d\",endj,score);");
    expr(dfp,"stop_reporting();");

    expr(dfp,"for(;;)");
    startbrace_tag(dfp,"while there are more special bits to recover");
    expr(dfp,"start_reporting(\"Special cell aln end   %%d:\",endj)");
    expr(dfp,"if( read_special_strip_%s(mat,0,endj,stopstate,&endj,&startstate,out) == FALSE )",gm->name);
    startbrace(dfp);
    warn_expr(dfp,"Problem in reading off special state system... going to return partial alignment");
    expr(dfp,"break;");
    closebrace(dfp);
    expr(dfp,"if( startstate == %s || endj <= 0)",gm->special[ss]->name);
    startbrace(dfp);
    expr(dfp,"log_full_error(REPORT,0,\"Recovered complete alignment\");");
    expr(dfp,"stop_reporting();");
    expr(dfp,"break;");
    closebrace(dfp);

    add_break(dfp);
    expr(dfp,"log_full_error(REPORT,0,\"Finished to %%d\",endj);");
    expr(dfp,"stop_reporting();");
    add_break(dfp);
    add_block_comment(dfp,"Ok... have to eat up another piece of matrix <sigh>");
    expr(dfp,"temp = startstate;");
    expr(dfp,"starti = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,temp,0);",gm->name);
    expr(dfp,"startj = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,temp,1);",gm->name);
    expr(dfp,"startstate = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,temp,2);",gm->name);
    expr(dfp,"stopi = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,temp,3);",gm->name);
    expr(dfp,"stopj = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,temp,4);",gm->name);
    expr(dfp,"stopstate = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,temp,5);",gm->name);
    add_break(dfp);
    add_block_comment(dfp,"Get out the score of this block. V. important!");
    expr(dfp,"temp = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,temp,6);",gm->name);
    
    expr(dfp,"totalj = stopj - startj;");
    expr(dfp,"donej  = 0;");
    expr(dfp,"start_reporting(\"Main matrix  aln [%%d,%%d]:\",startj,stopj)");
    expr(dfp,"if(full_dc_%s(mat,starti,startj,startstate,stopi,stopj,stopstate,out,&donej,totalj,dpenv) == FALSE)",gm->name);
    startbrace(dfp);
    expr(dfp,"warn(\"In the alignment %s [%%d,%%d][%%d,%%d], got a problem. Please report bug ... giving you back a partial alignment\",starti,startj,stopi,stopj);",gm->name);
    expr(dfp,"return out;");
    closebrace(dfp);
    add_break(dfp);
    
    add_block_comment(dfp,"now have to figure out which special we came from... yikes");

    expr(dfp,"max_matrix_to_special_%s(mat,starti,startj,startstate,temp,&stopi,&stopj,&stopstate,&temp,NULL);",gm->name);
    expr(dfp,"if( stopi == %s_READ_OFF_ERROR)",gm->name);
    startbrace(dfp);
    expr(dfp,"warn(\"In %s read off ending at %%d ... got a bad matrix to special read off... returning partial alignment\",startj);",gm->name);
    expr(dfp,"invert_PackAln(out);");
    expr(dfp,"recalculate_PackAln_%s(out,mat);",gm->name);
    expr(dfp,"return out;");
    closebrace(dfp);

    add_block_comment(dfp,"if at start, break, otherwise, back to eat another strip");

    

    
    expr(dfp,"if( stopstate == %s)",gm->special[ss]->name);
    startbrace(dfp);
    expr(dfp,"log_full_error(REPORT,0,\"Recovered complete alignment      \");");
    expr(dfp,"stop_reporting()");
    expr(dfp,"break;");
    closebrace(dfp);
    expr(dfp,"log_full_error(REPORT,0,\"Finished  alignment to %%d           \",startj);");
    expr(dfp,"stop_reporting();");
    expr(dfp,"endj = stopj;");
    add_block_comment(dfp,"stopstate is correct as it is");
    closebrace(dfp);

  }

  /*** clean up ***/

  expr(dfp,"invert_PackAln(out);");
  expr(dfp,"recalculate_PackAln_%s(out,mat);",gm->name);

  expr(dfp,"return out;");
  add_break(dfp);
  close_function(dfp);
  add_break(dfp);
}


/* Function:  one_shot_AlnRangeSet_func(dfp,gm)
 *
 * Descrip:    makes AlnRangeSet from small memory
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:         gm [UNKN ] Undocumented argument [GenericMatrix *]
 *
 */
# line 369 "dynashadow.dy"
void one_shot_AlnRangeSet_func(DYNFILE * dfp,GenericMatrix * gm)
{
  FuncInfo * fi;
  
  fi = FuncInfo_named_from_varstr(FI_CALLABLE,"AlnRangeSet_calculate_Small_%s",gm->name);
  add_line_to_Ftext(fi->ft,"This function calculates an alignment for %s structure in linear space",gm->name);
  add_line_to_Ftext(fi->ft,"If you want the full alignment, use /PackAln_calculate_Small_%s ",gm->name);
  add_line_to_Ftext(fi->ft,"If you have already got the full alignment, but want the range set, use /AlnRangeSet_from_PackAln_%s",gm->name);
  add_line_to_Ftext(fi->ft,"If you have got the small matrix but not the alignment, use /AlnRangeSet_from_%s ",gm->name);

  start_function_FuncInfo(fi,dfp,"AlnRangeSet * AlnRangeSet_calculate_Small_%s(%s * mat)",gm->name,gm->name);


  /*** function body ***/
  expr(dfp,"AlnRangeSet * out;");


  add_break(dfp);
  
  expr(dfp,"start_reporting(\"Find start end points: \");");
  expr(dfp,"dc_optimised_start_end_calc_%s(mat,NULL);",gm->name);
  expr(dfp,"log_full_error(REPORT,0,\"Calculated\");");

  add_break(dfp);

  expr(dfp,"out = AlnRangeSet_from_%s(mat);",gm->name);

  expr(dfp,"return out;");
  close_function(dfp);
  add_break(dfp);
}
  
  
# line 402 "dynashadow.dy"
void write_AlnRangeSet_build_func(DYNFILE * dfp,GenericMatrix * gm)
{
  register int s;
  FuncInfo * fi;
  
  fi = FuncInfo_named_from_varstr(FI_CALLABLE,"AlnRangeSet_from_%s",gm->name);
  add_line_to_Ftext(fi->ft,"This function reads off a start/end structure"); 
  add_line_to_Ftext(fi->ft,"for %s structure in linear space",gm->name);
  add_line_to_Ftext(fi->ft,"If you want the full alignment use");
  add_line_to_Ftext(fi->ft,"/PackAln_calculate_Small_%s ",gm->name);
  add_line_to_Ftext(fi->ft,"If you have not calculated the matrix use");
  add_line_to_Ftext(fi->ft,"/AlnRange_calculate_Small_%s",gm->name);



  for(s=0;s<gm->spec_len;s++)
    if( gm->special[s]->is_end == TRUE )
      break; 

  start_function_FuncInfo(fi,dfp,"AlnRangeSet * AlnRangeSet_from_%s(%s * mat)",gm->name,gm->name);
  expr(dfp,"AlnRangeSet * out;");
  expr(dfp,"AlnRange * temp;");
  expr(dfp,"int jpos;");
  expr(dfp,"int state;");
  


  add_break(dfp);
  expr(dfp,"if( mat->basematrix->type != BASEMATRIX_TYPE_SHADOW)");
  startbrace(dfp);
  warn_expr(dfp,"Bad error! - non shadow matrix type in AlnRangeSet_from_%s",gm->name);
  expr(dfp,"return NULL;");
  closebrace(dfp);
  add_break(dfp);
  
  expr(dfp,"out = AlnRangeSet_alloc_std();");
  

  add_block_comment(dfp,"Find the end position");

  expr(dfp,"out->score = start_end_find_end_%s(mat,&jpos);",gm->name);
  expr(dfp,"state = %s;",gm->special[s]->name);

  add_break(dfp);

  expr(dfp,"while( (temp = AlnRange_build_%s(mat,jpos,state,&jpos,&state)) != NULL)",gm->name);
  hang_expr(dfp,"add_AlnRangeSet(out,temp);");
  
  expr(dfp,"return out;");

  close_function(dfp);
  add_break(dfp);
}

# line 456 "dynashadow.dy"
void write_AlnRange_build_func(DYNFILE * dfp,GenericMatrix * gm)
{
  register int s;
  FuncInfo * fi;
  
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"AlnRange_build_%s",gm->name);
  add_line_to_Ftext(fi->ft,"This function calculates a single start/end set in linear space",gm->name);
  add_line_to_Ftext(fi->ft,"Really a sub-routine for /AlnRangeSet_from_PackAln_%s",gm->name);




  for(s=0;s<gm->spec_len;s++)
    if( gm->special[s]->is_start == TRUE )
      break; 

  if( s == gm->spec_len ) {
    warn("Ok... problem... the special length has no start. Unhappy!");
  }

  start_function_FuncInfo(fi,dfp,"AlnRange * AlnRange_build_%s(%s * mat,int stopj,int stopspecstate,int * startj,int * startspecstate)",gm->name,gm->name);
  expr(dfp,"AlnRange * out;");
  expr(dfp,"int jpos;");
  expr(dfp,"int state;");


  add_break(dfp);
  expr(dfp,"if( mat->basematrix->type != BASEMATRIX_TYPE_SHADOW)");
  startbrace(dfp);
  warn_expr(dfp,"Bad error! - non shadow matrix type in AlnRangeSet_from_%s",gm->name);
  expr(dfp,"return NULL;");
  closebrace(dfp);
  add_break(dfp);

  add_block_comment(dfp,"Assumme that we have specials (we should!). Read back along the specials till we have the finish point");

  expr(dfp,"if( read_special_strip_%s(mat,0,stopj,stopspecstate,&jpos,&state,NULL) == FALSE)",gm->name);
  startbrace(dfp);
  expr(dfp,"warn(\"In AlnRanger_build_%s alignment ending at %%d, unable to read back specials. Will (evenutally) return a partial range set... BEWARE!\",stopj);",gm->name);
  expr(dfp,"return NULL;");
  closebrace(dfp);

  expr(dfp,"if( state == %s || jpos <= 0)",gm->special[s]->name);
  hang_expr(dfp,"return NULL;");
  

  add_break(dfp);
  expr(dfp,"out = AlnRange_alloc();");
  add_break(dfp);
  expr(dfp,"out->starti = %s_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,0);",gm->name);
  expr(dfp,"out->startj = %s_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,1);",gm->name);
  expr(dfp,"out->startstate = %s_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,2);",gm->name);
  expr(dfp,"out->stopi = %s_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,3);",gm->name);
  expr(dfp,"out->stopj = %s_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,4);",gm->name);
  expr(dfp,"out->stopstate = %s_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,5);",gm->name);
  expr(dfp,"out->startscore = %s_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,6);",gm->name);
  expr(dfp,"out->stopscore = %s_DC_SHADOW_SPECIAL(mat,0,jpos,state);",gm->name);
  add_break(dfp);
  add_block_comment(dfp,"Now, we have to figure out where this state came from in the specials");
  expr(dfp,"max_matrix_to_special_%s(mat,out->starti,out->startj,out->startstate,out->startscore,&jpos,startj,startspecstate,&state,NULL);",gm->name);
  expr(dfp,"if( jpos == %s_READ_OFF_ERROR)",gm->name);
  startbrace(dfp);
  expr(dfp,"warn(\"In AlnRange_build_%s alignment ending at %%d, with aln range between %%d-%%d in j, unable to find source special, returning this range, but this could get tricky!\",stopj,out->startj,out->stopj);",gm->name);
  expr(dfp,"return out;");
  closebrace(dfp);

  add_break(dfp);

  add_block_comment(dfp,"Put in the correct score for startstate, from the special");
  expr(dfp,"out->startscore = %s_DC_SHADOW_SPECIAL(mat,0,*startj,*startspecstate);",gm->name);

  add_block_comment(dfp,"The correct j coords have been put into startj, startspecstate... so just return out");
  expr(dfp,"return out;");

  close_function(dfp);

  add_break(dfp);
}
  
/* Function:  write_dc_PackAln_build_func(dfp,gm)
 *
 * Descrip:    This functions is now defunct
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:         gm [UNKN ] Undocumented argument [GenericMatrix *]
 *
 */
# line 538 "dynashadow.dy"
void write_dc_PackAln_build_func(DYNFILE * dfp,GenericMatrix * gm)
{
  register int i;
  register int s;
  int ss;
  char buffer[MAXLINE];

  /*** yawn. There are better ways to do this! ***/

  for(s=0;s<gm->spec_len;s++)
    if( gm->special[s]->is_end == TRUE )
      break; 

  for(ss=0;ss<gm->spec_len;ss++) 
    if( gm->special[ss]->is_start == TRUE)
      break;

  sprintf(buffer,"PackAln * PackAln_dc_build_%s(%s %s, %s %s ",gm->name,
	  gm->query->element_type,gm->query->name,
	  gm->target->element_type,gm->target->name);
  
  for(i=0;i<gm->res_len;i++) {
    strcat(buffer,", ");
    strcat(buffer,gm->resource[i]->element_type);
    if( gm->resource[i]->isfunc == TRUE ) { continue; }
    strcat(buffer," ");
    strcat(buffer,gm->resource[i]->name);
  }
  
  

  strcat(buffer,")");
  
  
  start_function(dfp,buffer);
  expr(dfp,"int endj;");
  expr(dfp,"%s * mat;",gm->name);
  expr(dfp,"int score;");
  expr(dfp,"PackAln * out;");
  expr(dfp,"int starti;");
  expr(dfp,"int startj;");
  expr(dfp,"int startstate;");
  expr(dfp,"int stopi;");
  expr(dfp,"int stopj;");
  expr(dfp,"int stopstate;");
  expr(dfp,"int temp;");
  expr(dfp,"int donej;");
  add_end_comment(dfp,"This is for reporting, will be passed as a & arg in");
  expr(dfp,"int totalj;");
  add_end_comment(dfp,"This also is for reporting, but as is not changed, can be passed by value");

  add_break(dfp);

  expr(dfp,"out = PackAln_alloc_std();");
  expr(dfp,"mat = shadow_dc_alloc_%s(%s->%s,%s->%s);",gm->name,gm->query->name,gm->query_len,gm->target->name,gm->target_len);
  expr(dfp,"if(mat == NULL)");
  startbrace(dfp);
  warn_expr(dfp,"Unable to build test dc linear matrix for %s, unable to allocate memory",gm->name);
  expr(dfp,"return NULL;");
  closebrace(dfp);

  add_break(dfp);
  
  expr(dfp,"mat->%s = %s;",gm->query->name,gm->query->name);
  expr(dfp,"mat->%s = %s;",gm->target->name,gm->target->name);
  for(i=0;i<gm->res_len;i++)
    expr(dfp,"mat->%s = %s;",gm->resource[i]->name,gm->resource[i]->name);


  add_break(dfp);
  
  /** got to figure out start/end first **/

  expr(dfp,"start_reporting(\"Find start end points: \");");
  expr(dfp,"dc_optimised_start_end_calc_%s(mat);",gm->name);
  
  expr(dfp,"score = start_end_find_end_%s(mat,&endj);",gm->name);
  expr(dfp,"out->score = score;");
  expr(dfp,"stopstate = %s;\n",gm->special[s]->name);


  /*** ok, life is different if we have special to specials. ***/

  if( gm->specialtospecial == FALSE) {
    add_block_comment(dfp,"No special to specials: one matrix alignment: simply remove and get");
    expr(dfp,"starti = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,%s,0);",gm->name,gm->special[s]->name);
    expr(dfp,"startj = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,%s,1);",gm->name,gm->special[s]->name);
    expr(dfp,"startstate = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,%s,2);",gm->name,gm->special[s]->name);
    expr(dfp,"stopi = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,%s,3);",gm->name,gm->special[s]->name);
    expr(dfp,"stopj = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,%s,4);",gm->name,gm->special[s]->name);
    expr(dfp,"stopstate = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,%s,5);",gm->name,gm->special[s]->name);
    expr(dfp,"log_full_error(REPORT,0,\"[%%d,%%d][%%d,%%d] Score %%d\",starti,startj,stopi,stopj,score);");
    expr(dfp,"stop_reporting();");
    
    expr(dfp,"start_reporting(\"Recovering alignment: \");");
    
    add_break(dfp);
    add_block_comment(dfp,"Figuring how much j we have to align for reporting purposes");
    expr(dfp,"donej = 0;");
    expr(dfp,"totalj = stopj - startj;");
    
    expr(dfp,"full_dc_%s(mat,starti,startj,startstate,stopi,stopj,stopstate,out,&donej,totalj,dpenv);",gm->name);
    expr(dfp,"log_full_error(REPORT,0,\"Alignment recovered\");");
    expr(dfp,"stop_reporting();");
  }
  else {
    add_block_comment(dfp,"Special to specials: have to eat up in strip and then drop back to full_dc for intervening bits");
    expr(dfp,"log_full_error(REPORT,0,\"End at %%d Score %%d\",endj,score);");
    expr(dfp,"stop_reporting();");

    expr(dfp,"for(;;)");
    startbrace_tag(dfp,"while there are more special bits to recover");
    expr(dfp,"start_reporting(\"Special cell aln end   %%d:\",endj)");
    expr(dfp,"if( read_special_strip_%s(mat,0,endj,stopstate,&endj,&startstate,out) == FALSE )",gm->name);
    startbrace(dfp);
    warn_expr(dfp,"Problem in reading off special state system... going to return partial alignment");
    expr(dfp,"break;");
    closebrace(dfp);
    expr(dfp,"if( startstate == %s || endj <= 0)",gm->special[ss]->name);
    startbrace(dfp);
    expr(dfp,"log_full_error(REPORT,0,\"Recovered complete alignment\");");
    expr(dfp,"stop_reporting();");
    expr(dfp,"break;");
    closebrace(dfp);

    add_break(dfp);
    expr(dfp,"log_full_error(REPORT,0,\"Finished to %%d\",endj);");
    expr(dfp,"stop_reporting();");
    add_break(dfp);
    add_block_comment(dfp,"Ok... have to eat up another piece of matrix <sigh>");
    expr(dfp,"temp = startstate;");
    expr(dfp,"starti = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,temp,0);",gm->name);
    expr(dfp,"startj = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,temp,1);",gm->name);
    expr(dfp,"startstate = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,temp,2);",gm->name);
    expr(dfp,"stopi = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,temp,3);",gm->name);
    expr(dfp,"stopj = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,temp,4);",gm->name);
    expr(dfp,"stopstate = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,temp,5);",gm->name);
    add_break(dfp);
    add_block_comment(dfp,"Get out the score of this block. V. important!");
    expr(dfp,"temp = %s_DC_SHADOW_SPECIAL_SP(mat,0,endj,temp,6);",gm->name);
    
    expr(dfp,"totalj = stopj - startj;");
    expr(dfp,"donej  = 0;");
    expr(dfp,"start_reporting(\"Main matrix  aln [%%d,%%d]:\",startj,stopj)");
    expr(dfp,"if(full_dc_%s(mat,starti,startj,startstate,stopi,stopj,stopstate,out,&donej,totalj,dpenv) == FALSE)",gm->name);
    startbrace(dfp);
    expr(dfp,"warn(\"In the alignment %s [%%d,%%d][%%d,%%d], got a problem. Please report bug ... giving you back a partial alignment\",starti,startj,stopi,stopj);",gm->name);
    expr(dfp,"return out;");
    closebrace(dfp);
    add_break(dfp);
    
    add_block_comment(dfp,"now have to figure out which special we came from... yikes");

    expr(dfp,"max_matrix_to_special_%s(mat,starti,startj,startstate,temp,&stopi,&stopj,&stopstate,&temp,NULL);",gm->name);
    expr(dfp,"if( stopi == %s_READ_OFF_ERROR);",gm->name);
    startbrace(dfp);
    expr(dfp,"warn(\"In %s read off ending at %%d ... got a bad matrix to special read off... returning partial alignment\",startj);",gm->name);
    expr(dfp,"invert_PackAln(out);");
    expr(dfp,"return out;");
    closebrace(dfp);

    add_block_comment(dfp,"if at start, break, otherwise, back to eat another strip");

    

    
    expr(dfp,"if( stopstate == %s)",gm->special[ss]->name);
    startbrace(dfp);
    expr(dfp,"log_full_error(REPORT,0,\"Recovered complete alignment      \");");
    expr(dfp,"stop_reporting()");
    expr(dfp,"break;");
    closebrace(dfp);
    expr(dfp,"log_full_error(REPORT,0,\"Finished  alignment to %%d           \",startj);");
    expr(dfp,"stop_reporting();");
    expr(dfp,"endj = stopj;");
    add_block_comment(dfp,"stopstate is correct as it is");
    closebrace(dfp);

  }


    



  expr(dfp,"invert_PackAln(out);");

  expr(dfp,"free_%s(mat);",gm->name);
  expr(dfp,"return out;");
  add_break(dfp);
  close_function(dfp);
  add_break(dfp);
}
  
/* Function:  write_full_dc_func(dfp,gm)
 *
 * Descrip:    writes the main divide and conquor routine
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:         gm [UNKN ] Undocumented argument [GenericMatrix *]
 *
 */
# line 735 "dynashadow.dy"
void write_full_dc_func(DYNFILE * dfp,GenericMatrix * gm)
{
  ArgInfo * ai;
  FuncInfo * fi;
  
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"full_dc_%s",gm->name);
  add_line_to_Ftext(fi->ft,"The main divide-and-conquor routine. Basically, call /PackAln_calculate_small_%s",gm->name);
  add_line_to_Ftext(fi->ft,"Not this function, which is pretty hard core. ");
  add_line_to_Ftext(fi->ft,"Function is given start/end points (in main matrix) for alignment");
  add_line_to_Ftext(fi->ft,"It does some checks, decides whether start/end in j is small enough for explicit calc");
  add_line_to_Ftext(fi->ft,"  - if yes, calculates it, reads off into PackAln (out), adds the j distance to donej and returns TRUE");
  add_line_to_Ftext(fi->ft,"  - if no,  uses /do_dc_single_pass_%s to get mid-point",gm->name);
  add_line_to_Ftext(fi->ft,"             saves midpoint, and calls itself to do right portion then left portion");
  add_line_to_Ftext(fi->ft,"right then left ensures PackAln is added the 'right' way, ie, back-to-front");
  add_line_to_Ftext(fi->ft,"returns FALSE on any error, with a warning");

  ai =  ArgInfo_in_FuncInfo_from_varstr(fi,"mat");
  ai->desc=stringalloc("Matrix with small memory implementation");

  ai =  ArgInfo_in_FuncInfo_from_varstr(fi,"starti");
  ai->desc=stringalloc("Start position in i");
  ai =  ArgInfo_in_FuncInfo_from_varstr(fi,"startj");
  ai->desc=stringalloc("Start position in j");
  ai =  ArgInfo_in_FuncInfo_from_varstr(fi,"startstate");
  ai->desc=stringalloc("Start position state number");

  ai =  ArgInfo_in_FuncInfo_from_varstr(fi,"stopi");
  ai->desc=stringalloc("Stop position in i");
  ai =  ArgInfo_in_FuncInfo_from_varstr(fi,"stopj");
  ai->desc=stringalloc("Stop position in j");
  ai =  ArgInfo_in_FuncInfo_from_varstr(fi,"stopstate");
  ai->desc=stringalloc("Stop position state number");

  ai =  ArgInfo_in_FuncInfo_from_varstr(fi,"out");
  ai->desc=stringalloc("PackAln structure to put alignment into");

  ai =  ArgInfo_in_FuncInfo_from_varstr(fi,"donej");
  ai->desc=stringalloc("pointer to a number with the amount of alignment done");

  ai =  ArgInfo_in_FuncInfo_from_varstr(fi,"totalj");
  ai->desc=stringalloc("total amount of alignment to do (in j coordinates)");


  start_function_FuncInfo(fi,dfp,"boolean full_dc_%s(%s * mat,int starti,int startj,int startstate,int stopi,int stopj,int stopstate,PackAln * out,int * donej,int totalj,DPEnvelope * dpenv)",gm->name,gm->name);
  expr(dfp,"int lstarti;");
  expr(dfp,"int lstartj;");
  expr(dfp,"int lstate;");


  add_break(dfp);
  expr(dfp,"if( mat->basematrix->type != BASEMATRIX_TYPE_SHADOW)");
  startbrace(dfp);
  warn_expr(dfp,"*Very* bad error! - non shadow matrix type in full_dc_%s",gm->name);
  expr(dfp,"return FALSE;");
  closebrace(dfp);
  add_break(dfp);

  expr(dfp,"if( starti == -1 || startj == -1 || startstate == -1 || stopi == -1 || stopstate == -1)");
  startbrace(dfp);
  expr(dfp,"warn(\"In full dc program, passed bad indices, indices passed were %%d:%%d[%%d] to %%d:%%d[%%d]\\n\",starti,startj,startstate,stopi,stopj,stopstate);");
  expr(dfp,"return FALSE");
  closebrace(dfp);

  add_break(dfp);

/***
  expr(dfp,"fprintf(stdout,\"Calling with %%d,%%d[%%d] to %%d,%%d[%%d]\\n\",starti,startj,startstate,stopi,stopj,stopstate);");
***/

  expr(dfp,"if( stopj - startj < %d)",gm->window_j*5);
  startbrace(dfp);
/**
  expr(dfp,"fprintf(stdout,\"Got to stopping point at %%d:%%d %%d:%%d\\n\",starti,startj,stopi,stopj);");
**/
  expr(dfp,"log_full_error(REPORT,0,\"[%%d,%%d][%%d,%%d] Explicit read off\",starti,startj,stopi,stopj);");
 
  add_end_comment(dfp,"Build hidden explicit matrix");

  expr(dfp,"calculate_hidden_%s(mat,starti,startj,startstate,stopi,stopj,stopstate,dpenv)",gm->name);
  expr(dfp,"*donej += (stopj - startj);");
  add_end_comment(dfp,"Now read it off into out");
  expr(dfp,"if( read_hidden_%s(mat,starti,startj,startstate,stopi,stopj,stopstate,out) == FALSE)",gm->name);
  startbrace(dfp);
  expr(dfp,"warn(\"In full dc, at %%d:%%d,%%d:%%d got a bad hidden explicit read off... \",starti,startj,stopi,stopj);");
  expr(dfp,"return FALSE");
  closebrace(dfp);
  expr(dfp,"return TRUE;");
  closebrace(dfp);
  add_break(dfp);

  add_end_comment(dfp,"In actual divide and conquor");


  expr(dfp,"if( do_dc_single_pass_%s(mat,starti,startj,startstate,stopi,stopj,stopstate,dpenv,(int)(*donej*100)/totalj) == FALSE)",gm->name);
  startbrace(dfp);
  expr(dfp,"warn(\"In divide and conquor for %s, at bound %%d:%%d to %%d:%%d, unable to calculate midpoint. Problem!\",starti,startj,stopi,stopj);",gm->name);
  expr(dfp,"return FALSE;");
  closebrace(dfp);

/***
  expr(dfp,"for(i=0;i<6;i++)");
  startbrace(dfp);
  expr(dfp,"printf(\"[%%d:%%d,%%d:%%d] %%d: %%d\\n\",starti,startj,stopi,stopj,i,%s_DC_SHADOW_MATRIX_SP(mat,stopi,stopj,stopstate,i));",gm->name);
  closebrace(dfp);
***/

  add_break(dfp);

  add_end_comment(dfp,"Ok... now we have to call on each side of the matrix");
  add_end_comment(dfp,"We have to retrieve left hand side positions, as they will be vapped by the time we call LHS");
  expr(dfp,"lstarti= %s_DC_SHADOW_MATRIX_SP(mat,stopi,stopj,stopstate,0)",gm->name);
  expr(dfp,"lstartj= %s_DC_SHADOW_MATRIX_SP(mat,stopi,stopj,stopstate,1)",gm->name);
  expr(dfp,"lstate = %s_DC_SHADOW_MATRIX_SP(mat,stopi,stopj,stopstate,2)",gm->name);

  add_break(dfp);
  add_end_comment(dfp,"Call on right hand side: this lets us do the correct read off");
  expr(dfp,"if( full_dc_%s(mat,%s_DC_SHADOW_MATRIX_SP(mat,stopi,stopj,stopstate,3),%s_DC_SHADOW_MATRIX_SP(mat,stopi,stopj,stopstate,4),%s_DC_SHADOW_MATRIX_SP(mat,stopi,stopj,stopstate,5),stopi,stopj,stopstate,out,donej,totalj,dpenv) == FALSE)",gm->name,gm->name,gm->name,gm->name);
  startbrace(dfp);
  add_end_comment(dfp,"Warning already issued, simply chained back up to top");
  expr(dfp,"return FALSE");
  closebrace(dfp);

  add_end_comment(dfp,"Call on left hand side");

  expr(dfp,"if( full_dc_%s(mat,starti,startj,startstate,lstarti,lstartj,lstate,out,donej,totalj,dpenv) == FALSE)",gm->name);
  startbrace(dfp);
  add_end_comment(dfp,"Warning already issued, simply chained back up to top");
  expr(dfp,"return FALSE");
  closebrace(dfp);

  add_break(dfp);

  expr(dfp,"return TRUE");

  close_function(dfp);
  add_break(dfp);
}

# line 873 "dynashadow.dy"
void write_single_dc_pass_func(DYNFILE * dfp,GenericMatrix * gm)
{
  FuncInfo * fi;
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"do_dc_single_pass_%s",gm->name);

  start_function_FuncInfo(fi,dfp,"boolean do_dc_single_pass_%s(%s * mat,int starti,int startj,int startstate,int stopi,int stopj,int stopstate,DPEnvelope * dpenv,int perc_done)",gm->name,gm->name);
  expr(dfp,"int halfj;");


  expr(dfp,"halfj = startj + ((stopj - startj)/2);");
/**
  expr(dfp,"fprintf(stdout,\"Pass with %%d,%%d[%%d] %%d,%%d[%%d] halfj %%d\\n\",starti,startj,startstate,stopi,stopj,stopstate,halfj);");
**/

  add_break(dfp);


  expr(dfp,"init_dc_%s(mat);",gm->name);
  add_break(dfp);
  expr(dfp,"%s_DC_SHADOW_MATRIX(mat,starti,startj,startstate) = 0;",gm->name);
  expr(dfp,"run_up_dc_%s(mat,starti,stopi,startj,halfj-1,dpenv,perc_done)",gm->name);
  expr(dfp,"push_dc_at_merge_%s(mat,starti,stopi,halfj,&halfj,dpenv)",gm->name);
  expr(dfp,"follow_on_dc_%s(mat,starti,stopi,halfj,stopj,dpenv,perc_done)",gm->name);
  expr(dfp,"return TRUE;");
  close_function(dfp);
  add_break(dfp);
}


# line 902 "dynashadow.dy"
void write_push_dc_func(DYNFILE * dfp,GenericMatrix * gm)
{
  int s;
  int r;
  int i;
  FuncInfo * fi;
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"push_dc_at_merge_%s",gm->name);

  start_function_FuncInfo(fi,dfp,"void push_dc_at_merge_%s(%s * mat,int starti,int stopi,int startj,int * stopj,DPEnvelope * dpenv)",gm->name,gm->name);
  expr(dfp,"register int i;");
  expr(dfp,"register int j;");
  expr(dfp,"register int k;");
  expr(dfp,"register int count;");
  expr(dfp,"register int mergej;");
  add_end_comment(dfp,"Sources below this j will be stamped by triples");
  expr(dfp,"register int score;");
  expr(dfp,"register int temp;");

  add_break(dfp);

  expr(dfp,"mergej = startj -1");
  expr(dfp,"for(count=0,j=startj;count<%d;count++,j++)",gm->window_j);
  startbrace(dfp);
  expr(dfp,"for(i=starti;i<=stopi;i++)");
  startbrace(dfp);


  expr(dfp,"if( dpenv != NULL && is_in_DPEnvelope(dpenv,i,j) == FALSE )");
  startbrace_tag(dfp,"Is not in envelope");

  for(i=0;i<gm->len;i++) {
    expr(dfp,"%s_DC_SHADOW_MATRIX(mat,i,j,%s) = NEGI",gm->name,gm->state[i]->name);
    expr(dfp,"%s_DC_SHADOW_MATRIX_SP(mat,i,j,%s,0) = (-100);",gm->name,gm->state[i]->name);
    expr(dfp,"%s_DC_SHADOW_MATRIX_SP(mat,i,j,%s,1) = (-100);",gm->name,gm->state[i]->name);
  }
  expr(dfp,"continue;");
  closebrace(dfp);


  for(s=0;s<gm->len;s++) {
    auto CellState * state;
    state = gm->state[s];

    add_break(dfp);
    add_block_comment(dfp,"For state %s, pushing when j - offj <= mergej",state->name);

    expr(dfp,"score = %s_DC_SHADOW_%s(mat,i-%d,j-%d,%s) + %s",gm->name,state->source[0]->isspecial == TRUE ? "SPECIAL" : "MATRIX",
	 state->source[0]->offi,state->source[0]->offj,state->source[0]->state_source,state->source[0]->calc_expr);
    expr(dfp,"if( j - %d <= mergej)",state->source[0]->offj);
    startbrace(dfp);
    expr(dfp,"%s_DC_SHADOW_MATRIX_SP(mat,i,j,%s,0) = i-%d;",gm->name,state->name,state->source[0]->offi);
    expr(dfp,"%s_DC_SHADOW_MATRIX_SP(mat,i,j,%s,1) = j-%d;",gm->name,state->name,state->source[0]->offj);
    expr(dfp,"%s_DC_SHADOW_MATRIX_SP(mat,i,j,%s,2) = %s;",gm->name,state->name,state->source[0]->state_source);
    expr(dfp,"%s_DC_SHADOW_MATRIX_SP(mat,i,j,%s,3) = i;",gm->name,state->name,state->source[0]->offi);
    expr(dfp,"%s_DC_SHADOW_MATRIX_SP(mat,i,j,%s,4) = j;",gm->name,state->name,state->source[0]->offj);
    expr(dfp,"%s_DC_SHADOW_MATRIX_SP(mat,i,j,%s,5) = %s;",gm->name,state->name,state->name);
    closebrace(dfp);
    expr(dfp,"else");
    startbrace(dfp);
    expr(dfp,"for(k=0;k<7;k++)");
    hang_expr(dfp,"%s_DC_SHADOW_MATRIX_SP(mat,i,j,%s,k) = %s_DC_SHADOW_%s_SP(mat,i - %d,j - %d,%s,k);",gm->name,
	      state->name,gm->name,state->source[0]->isspecial == TRUE ? "SPECIAL" : "MATRIX",
	      state->source[0]->offi,state->source[0]->offj,state->source[0]->state_source);
    closebrace(dfp);



    for(r=1;r<state->len;r++) {
      if( state->source[r]->isspecial == TRUE )
	continue;

      add_break(dfp);

      expr(dfp,"temp = %s_DC_SHADOW_%s(mat,i-%d,j-%d,%s) + %s",gm->name,state->source[r]->isspecial == TRUE ? "SPECIAL" : "MATRIX",
	   state->source[r]->offi,state->source[r]->offj,state->source[r]->state_source,state->source[r]->calc_expr);

      expr(dfp,"if( temp > score)");
      startbrace(dfp);
      expr(dfp,"score = temp;");
      add_break(dfp);
      expr(dfp,"if( j - %d <= mergej)",state->source[r]->offj);
      startbrace(dfp);

      expr(dfp,"%s_DC_SHADOW_MATRIX_SP(mat,i,j,%s,0) = i-%d;",gm->name,state->name,state->source[r]->offi);
      expr(dfp,"%s_DC_SHADOW_MATRIX_SP(mat,i,j,%s,1) = j-%d;",gm->name,state->name,state->source[r]->offj);
      expr(dfp,"%s_DC_SHADOW_MATRIX_SP(mat,i,j,%s,2) = %s;",gm->name,state->name,state->source[r]->state_source);
      expr(dfp,"%s_DC_SHADOW_MATRIX_SP(mat,i,j,%s,3) = i;",gm->name,state->name,state->source[r]->offi);
      expr(dfp,"%s_DC_SHADOW_MATRIX_SP(mat,i,j,%s,4) = j;",gm->name,state->name,state->source[r]->offj);
      expr(dfp,"%s_DC_SHADOW_MATRIX_SP(mat,i,j,%s,5) = %s;",gm->name,state->name,state->name);
      closebrace(dfp);
      expr(dfp,"else");
      startbrace(dfp);
      expr(dfp,"for(k=0;k<7;k++)");
      hang_expr(dfp,"%s_DC_SHADOW_MATRIX_SP(mat,i,j,%s,k) = %s_DC_SHADOW_%s_SP(mat,i - %d,j - %d,%s,k);",gm->name,
		state->name,gm->name,state->source[r]->isspecial == TRUE ? "SPECIAL" : "MATRIX",
		state->source[r]->offi,state->source[r]->offj,state->source[r]->state_source);
      closebrace(dfp);
      closebrace(dfp);
    }

    /** ok, put in source independent score (if any!) **/

    add_block_comment(dfp,"Add any movement independant score");

    if( state->calc_expr != NULL )
      expr(dfp,"score += %s",state->calc_expr);

    expr(dfp,"%s_DC_SHADOW_MATRIX(mat,i,j,%s) = score",gm->name,state->name);


    /*** I don't need to look at specials ***/

    /***

    for(r=0;r<gm->spec_len;r++) {
      auto CellState * spec;
      spec = gm->special[r];

      for(t=0;t<spec->len;t++) {
	if( strcmp(spec->source[t]->state_source,state->name) == 0 ) {
	  
	  add_break(dfp);
	  add_block_comment(dfp,"state %s is a source for special %s... better update",state->name,spec->name);

	  expr(dfp,"temp = score + (%s) + (%s) ",spec->source[t]->calc_expr,spec->calc_expr == NULL ? "0" : spec->calc_expr);
	  expr(dfp,"if( temp > %s_DC_SHADOW_SPECIAL(mat,i,j,%s) )",gm->name,spec->name);
	  startbrace(dfp);
	  expr(dfp,"%s_DC_SHADOW_SPECIAL(mat,i,j,%s) = temp",gm->name,spec->name);
	  expr(dfp,"for(k=0;k<7;k++)");
	  hang_expr(dfp,"%s_DC_SHADOW_SPECIAL_SP(mat,i,j,%s,k) = %s_DC_SHADOW_MATRIX_SP(mat,i,j,%s,k);",gm->name,
		    spec->name,gm->name,state->name);
	  closebrace(dfp);
	} end of if this is a special source 
      }end of t over all sources for this special 
    }end of r over all specials 
    
    ***/



    add_block_comment(dfp,"Finished with state %s",state->name);
 
  } /* end of s over state */

  closebrace(dfp); /* close i */
  closebrace(dfp); /* close j */

  add_block_comment(dfp,"Put back j into * stop j so that calling function gets it correct");

  expr(dfp,"if( stopj == NULL)");
  hang_expr(dfp,"warn(\"Bad news... NULL stopj pointer in push dc function. This means that calling function does not know how many cells I have done!\");");
  expr(dfp,"else");
  hang_expr(dfp,"*stopj = j");
  
  add_break(dfp);
  
  expr(dfp,"return");
  close_function(dfp);
  add_break(dfp);
}

# line 1063 "dynashadow.dy"
void write_init_dc_func(DYNFILE * dfp,GenericMatrix * gm)
{
  int s;
  FuncInfo * fi;
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"init_dc_%s",gm->name);

  start_function_FuncInfo(fi,dfp,"void init_dc_%s(%s * mat)",gm->name,gm->name);
  expr(dfp,"register int i;");
  expr(dfp,"register int j;");
  expr(dfp,"register int k;");

  add_break(dfp);
  expr(dfp,"for(j=0;j<%d;j++)",gm->window_j+2);
  startbrace(dfp);
  expr(dfp,"for(i=(-%d);i<mat->%s->%s;i++)",gm->window_i,gm->query->name,gm->query_len);
  startbrace(dfp);
  for(s=0;s<gm->len;s++) {
    expr(dfp,"%s_DC_SHADOW_MATRIX(mat,i,j,%s) = %s;",gm->name,gm->state[s]->name,gm->state[s]->def_score);
  }
  expr(dfp,"for(k=0;k<7;k++)");
  startbrace(dfp);
  for(s=0;s<gm->len;s++) {
    expr(dfp,"%s_DC_SHADOW_MATRIX_SP(mat,i,j,%s,k) = (-1);",gm->name,gm->state[s]->name);
  }
  closebrace(dfp);
  closebrace(dfp);
  closebrace(dfp);

  add_break(dfp);

  expr(dfp,"return");
  close_function(dfp);
  add_break(dfp);
}

  
# line 1099 "dynashadow.dy"
void write_up_to_dc_func(DYNFILE * dfp,GenericMatrix * gm)
{
  FuncInfo * fi;
  int i;
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"run_up_dc_%s",gm->name);

  start_function_FuncInfo(fi,dfp,"void run_up_dc_%s(%s * mat,int starti,int stopi,int startj,int stopj,DPEnvelope * dpenv,int perc_done)",gm->name,gm->name);
  expr(dfp,"register int i;");
  expr(dfp,"register int j;");
  expr(dfp,"register int score;");
  expr(dfp,"register int temp;");
  expr(dfp,"long int total;");
  expr(dfp,"long int num;");

  add_break(dfp);
  expr(dfp,"total = (stopi - starti+1) * (stopj - startj+1);");
  expr(dfp,"if( total <= 0 )");
  hang_expr(dfp,"total = 1;");
  expr(dfp,"num = 0");
  add_break(dfp);


  expr(dfp,"for(j=startj;j<=stopj;j++)");
  startbrace_tag(dfp,"for each valid j column");
  expr(dfp,"for(i=starti;i<=stopi;i++)");
  startbrace_tag(dfp,"this is strip");
  expr(dfp,"if( j == startj && i == starti)");
  hang_expr(dfp,"continue;");
  expr(dfp,"num++;");

  expr(dfp,"if( dpenv != NULL && is_in_DPEnvelope(dpenv,i,j) == FALSE )");
  startbrace_tag(dfp,"Is not in envelope");

  for(i=0;i<gm->len;i++) {
    expr(dfp,"%s_DC_SHADOW_MATRIX(mat,i,j,%s) = NEGI",gm->name,gm->state[i]->name);
  }
  expr(dfp,"continue;");
  closebrace(dfp);

  expr(dfp,"if( num %% 1000 == 0 )");
  hang_expr(dfp,"log_full_error(REPORT,0,\"[%%d%%%%%%%% done]Before mid-j %%5d Cells done %%d%%%%%%%%\",perc_done,stopj,(num*100)/total)");

  /** don't use specials, as we are guarenteed an alignment outisde them **/

  write_score_block(dfp,gm,"DC_SHADOW_MATRIX","mat","DC_SHADOW_SPECIAL",FALSE); 
  closebrace(dfp);
  closebrace(dfp);

  add_break(dfp);

  close_function(dfp);

}
  

# line 1154 "dynashadow.dy"
void write_follow_on_dc_func(DYNFILE * dfp,GenericMatrix * gm)
{
  int i;
  FuncInfo * fi;
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"follow_on_dc_%s",gm->name);

  start_function_FuncInfo(fi,dfp,"void follow_on_dc_%s(%s * mat,int starti,int stopi,int startj,int stopj,DPEnvelope * dpenv,int perc_done)",gm->name,gm->name);
  expr(dfp,"int i;");
  expr(dfp,"int j;");
  expr(dfp,"int k;");
  expr(dfp,"int score;");
  expr(dfp,"int temp;");
  expr(dfp,"int localshadow[7];");
  expr(dfp,"long int total;");
  expr(dfp,"long int num;");

  add_break(dfp);
  expr(dfp,"total = (stopi - starti+1) * (stopj - startj+1);");
  expr(dfp,"num = 0");
  add_break(dfp);

  expr(dfp,"for(j=startj;j<=stopj;j++)");
  startbrace_tag(dfp,"for each valid j column");
  expr(dfp,"for(i=starti;i<=stopi;i++)");
  startbrace_tag(dfp,"this is strip");
  expr(dfp,"num++;");

  expr(dfp,"if( dpenv != NULL && is_in_DPEnvelope(dpenv,i,j) == FALSE )");
  startbrace_tag(dfp,"Is not in envelope");
  for(i=0;i<gm->len;i++) {
    expr(dfp,"%s_DC_SHADOW_MATRIX(mat,i,j,%s) = NEGI",gm->name,gm->state[i]->name);
  }
  expr(dfp,"continue;");
  closebrace(dfp);

  expr(dfp,"if( num %% 1000 == 0 )");
  hang_expr(dfp,"log_full_error(REPORT,0,\"[%%d%%%%%%%% done]After  mid-j %%5d Cells done %%d%%%%%%%%\",perc_done,startj,(num*100)/total)");


  write_main_shadow_block(dfp,gm,"DC_SHADOW_MATRIX","mat","DC_SHADOW_SPECIAL","DC_SHADOW_MATRIX_SP","DC_SHADOW_SPECIAL_SP",7,FALSE,FALSE,0,FALSE);
  closebrace(dfp);

  /** no specials, as we are guarenteed to be outside them **/

  closebrace(dfp);

  add_break(dfp);

  close_function(dfp);

}

# line 1206 "dynashadow.dy"
void write_special_strip_read_func(DYNFILE * dfp,GenericMatrix * gm)
{
  int ss;
  FuncInfo * fi;


  for(ss=0;ss<gm->spec_len;ss++) 
    if( gm->special[ss]->is_start == TRUE)
      break;

  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"read_special_strip_%s",gm->name);

  start_function_FuncInfo(fi,dfp,"boolean read_special_strip_%s(%s * mat,int stopi,int stopj,int stopstate,int * startj,int * startstate,PackAln * out)",gm->name,gm->name);
  expr(dfp,"int i;");
  expr(dfp,"int j;");
  expr(dfp,"int state;");
  expr(dfp,"int cellscore;");
  expr(dfp,"int isspecial;");
  expr(dfp,"PackAlnUnit * pau;");

  add_break(dfp);

  add_block_comment(dfp,"stop position is on the path");

  expr(dfp,"i = stopi;");
  expr(dfp,"j = stopj;");
  expr(dfp,"state= stopstate;");
  expr(dfp,"isspecial = TRUE;");

  add_break(dfp);

  add_block_comment(dfp,"Loop until state has the same j as its stop in shadow pointers");
  add_block_comment(dfp,"This will be the state is came out from, OR it has hit !start");
  add_block_comment(dfp,"We may not want to get the alignment, in which case out will be NULL");

  expr(dfp,"while( j > %s_DC_SHADOW_SPECIAL_SP(mat,i,j,state,4) && state != %s)",gm->name,gm->special[ss]->name);
  startbrace_tag(dfp,"while more specials to eat up");
  add_block_comment(dfp,"Put away current state, if we should");
  expr(dfp,"if(out != NULL)");
  startbrace(dfp);
  expr(dfp,"pau = PackAlnUnit_alloc();");
  add_end_comment(dfp,"Should deal with memory overflow");
  expr(dfp,"pau->i = i;");
  expr(dfp,"pau->j = j;");
  expr(dfp,"pau->state =  state + %d;",gm->len);
  expr(dfp,"add_PackAln(out,pau);");
  closebrace(dfp);
  add_break(dfp);

  expr(dfp,"max_special_strip_%s(mat,i,j,state,isspecial,&i,&j,&state,&isspecial,&cellscore);",gm->name);
  expr(dfp,"if( i == %s_READ_OFF_ERROR)",gm->name);
  startbrace(dfp);
  warn_expr(dfp,"In special strip read %s, got a bad read off error. Sorry!",gm->name);
  expr(dfp,"return FALSE;");
  closebrace(dfp);
  closebrace(dfp);

  add_break(dfp);
  add_block_comment(dfp,"check to see we have not gone too far!");

  expr(dfp,"if( state != %s && j < %s_DC_SHADOW_SPECIAL_SP(mat,i,j,state,4))",gm->special[ss]->name,gm->name);
  startbrace(dfp);
  expr(dfp,"warn(\"In special strip read %s, at special [%%d] state [%%d] overshot!\",j,state);",gm->name);
  expr(dfp,"return FALSE;");
  closebrace(dfp);
  add_block_comment(dfp,"Put away last state");
  expr(dfp,"if(out != NULL)");
  startbrace(dfp);
  expr(dfp,"pau = PackAlnUnit_alloc();");
  add_end_comment(dfp,"Should deal with memory overflow");
  expr(dfp,"pau->i = i;");
  expr(dfp,"pau->j = j;");
  expr(dfp,"pau->state =  state + %d;",gm->len);
  expr(dfp,"add_PackAln(out,pau);");
  closebrace(dfp);
  add_break(dfp);

  add_block_comment(dfp,"Put away where we are in startj and startstate");
  expr(dfp,"*startj = j;");
  expr(dfp,"*startstate = state;");

  expr(dfp,"return TRUE;");
  close_function(dfp);

  add_break(dfp);
}


# line 1294 "dynashadow.dy"
void write_hidden_read_func(DYNFILE * dfp,GenericMatrix * gm)
{
  FuncInfo * fi;
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"read_hidden_%s",gm->name);

  start_function_FuncInfo(fi,dfp,"boolean read_hidden_%s(%s * mat,int starti,int startj,int startstate,int stopi,int stopj,int stopstate,PackAln * out)",gm->name,gm->name);
  expr(dfp,"int i;");
  expr(dfp,"int j;");
  expr(dfp,"int state;");
  expr(dfp,"int cellscore;");
  expr(dfp,"int isspecial;");
  add_block_comment(dfp,"We don't need hiddenj here, 'cause matrix access handled by max funcs");
  expr(dfp,"PackAlnUnit * pau;");

  add_break(dfp);

  add_block_comment(dfp,"stop position is on the path");

  expr(dfp,"i = stopi;");
  expr(dfp,"j = stopj;");
  expr(dfp,"state= stopstate;");
  expr(dfp,"isspecial = FALSE;");

  add_break(dfp);

  expr(dfp,"while( i >= starti && j >= startj)");
  startbrace(dfp);
  add_block_comment(dfp,"Put away current i,j,state");
  expr(dfp,"pau = PackAlnUnit_alloc();");
  add_end_comment(dfp,"Should deal with memory overflow");
  expr(dfp,"pau->i = i;");
  expr(dfp,"pau->j = j;");
  expr(dfp,"pau->state =  state;");
/**  expr(dfp,"fprintf(stdout,\"\\tPau unit %%d,%%d,%%d\\n\",i,j,state);"); **/
  expr(dfp,"add_PackAln(out,pau);");
  add_break(dfp);
  expr(dfp,"max_hidden_%s(mat,startj,i,j,state,isspecial,&i,&j,&state,&isspecial,&cellscore);",gm->name);
  add_break(dfp);
  expr(dfp,"if( i == %s_READ_OFF_ERROR)",gm->name);
  startbrace(dfp);
  expr(dfp,"warn(\"In %s hidden read off, between %%d:%%d,%%d:%%d - at got bad read off. Problem!\",starti,startj,stopi,stopj);",gm->name);
  expr(dfp,"return FALSE;");
  closebrace(dfp);
  add_break(dfp);
  expr(dfp,"if( i == starti && j == startj && state == startstate)");
  startbrace(dfp);

  add_end_comment(dfp,"Put away final state (start of this block)");

  expr(dfp,"pau = PackAlnUnit_alloc();");
  add_end_comment(dfp,"Should deal with memory overflow");
  expr(dfp,"pau->i = i;");
  expr(dfp,"pau->j = j;");
  expr(dfp,"pau->state =  state;");
  expr(dfp,"add_PackAln(out,pau);");

  hang_expr(dfp,"return TRUE;");
  closebrace(dfp);

  expr(dfp,"if( i == starti && j == startj)");
  startbrace(dfp);
  expr(dfp,"warn(\"In %s hidden read off, between %%d:%%d,%%d:%%d - hit start cell, but not in start state. Can't be good!.\",starti,startj,stopi,stopj);",gm->name);
  expr(dfp,"return FALSE;");
  closebrace(dfp);
  
  closebrace(dfp); /* back to while */

  
  expr(dfp,"warn(\"In %s hidden read off, between %%d:%%d,%%d:%%d - gone past start cell (now in %%d,%%d,%%d), can't be good news!.\",starti,startj,stopi,stopj,i,j,state);",gm->name);
  expr(dfp,"return FALSE;");

  close_function(dfp);
  add_break(dfp);
}
  
  

  

# line 1373 "dynashadow.dy"
void write_hidden_calc_func(DYNFILE * dfp,GenericMatrix * gm)
{
  FuncInfo * fi;
  int i;
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"calculate_hidden_%s",gm->name);

  start_function_FuncInfo(fi,dfp,"void calculate_hidden_%s(%s * mat,int starti,int startj,int startstate,int stopi,int stopj,int stopstate,DPEnvelope * dpenv)",gm->name,gm->name);
  expr(dfp,"register int i;");
  expr(dfp,"register int j;");
  expr(dfp,"register int score;");
  expr(dfp,"register int temp;");
  expr(dfp,"register int hiddenj;");
  
  add_break(dfp);
  expr(dfp,"hiddenj = startj;");

  add_break(dfp);
  expr(dfp,"init_hidden_%s(mat,starti,startj,stopi,stopj)",gm->name);
  add_break(dfp);

  expr(dfp,"%s_HIDDEN_MATRIX(mat,starti,startj,startstate) = 0;",gm->name);
  add_break(dfp);
  expr(dfp,"for(j=startj;j<=stopj;j++)");
  startbrace(dfp);
  expr(dfp,"for(i=starti;i<=stopi;i++)");
  startbrace(dfp);
  add_block_comment(dfp,"Should *not* do very first cell as this is the one set to zero in one state!");
  expr(dfp,"if( i == starti && j == startj )");
  hang_expr(dfp,"continue;");

  expr(dfp,"if( dpenv != NULL && is_in_DPEnvelope(dpenv,i,j) == FALSE )");
  startbrace_tag(dfp,"Is not in envelope");

  for(i=0;i<gm->len;i++) {
    expr(dfp,"%s_HIDDEN_MATRIX(mat,i,j,%s) = NEGI",gm->name,gm->state[i]->name);
  }
  expr(dfp,"continue;");
  closebrace(dfp);
  
  write_score_block(dfp,gm,"HIDDEN_MATRIX","mat","HIDDEN_SPECIAL",FALSE);

  closebrace(dfp);
  closebrace(dfp);

  add_break(dfp);

  expr(dfp,"return;");

  close_function(dfp);
  add_break(dfp);
}
  

# line 1426 "dynashadow.dy"
void write_hidden_init_func(DYNFILE * dfp,GenericMatrix * gm)
{
  register int s;
  FuncInfo * fi;
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"init_hidden_%s",gm->name);

  start_function_FuncInfo(fi,dfp,"void init_hidden_%s(%s * mat,int starti,int startj,int stopi,int stopj)",gm->name,gm->name);
  expr(dfp,"register int i;");
  expr(dfp,"register int j;");
  expr(dfp,"register int hiddenj;");
  
  add_break(dfp);
  expr(dfp,"hiddenj = startj;");

  expr(dfp,"for(j=(startj-%d);j<=stopj;j++)",gm->window_j);
  startbrace(dfp);
  expr(dfp,"for(i=(starti-%d);i<=stopi;i++)",gm->window_i);
  startbrace(dfp);

  for(s=0;s<gm->len;s++) {
    expr(dfp,"%s_HIDDEN_MATRIX(mat,i,j,%s) = %s;\n",gm->name,gm->state[s]->name,gm->state[s]->def_score);
  }

  closebrace(dfp);
  closebrace(dfp);

  add_break(dfp);

  expr(dfp,"return");
  close_function(dfp);
  add_break(dfp);
}


# line 1460 "dynashadow.dy"
void write_hidden_max_func(DYNFILE * dfp,GenericMatrix * gm)
{
  FuncInfo * fi;
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"max_hidden_%s",gm->name);

  start_function_FuncInfo(fi,dfp,"int max_hidden_%s(%s * mat,int hiddenj,int i,int j,int state,boolean isspecial,int * reti,int * retj,int * retstate,boolean * retspecial,int * cellscore)",gm->name,gm->name);
  
  expr(dfp,"register int temp;");
  expr(dfp,"register int cscore;");
  
  add_break(dfp);
  
  expr(dfp,"*reti = (*retj) = (*retstate) = %s_READ_OFF_ERROR;",gm->name);
  
  add_break(dfp);	
  
  expr(dfp,"if( i < 0 || j < 0 || i > mat->%s->%s || j > mat->%s->%s)",gm->query->name,
       gm->query_len,gm->target->name,gm->target_len);
  startbrace(dfp);
  expr(dfp,"warn(\"In %s matrix special read off - out of bounds on matrix [i,j is %%d,%%d state %%d in standard matrix]\",i,j,state);",gm->name);
  expr(dfp,"return -1;");
  closebrace(dfp);

  write_max_calc_block(dfp,gm,"HIDDEN_MATRIX","HIDDEN_SPECIAL",FALSE,FALSE);
  
  close_function(dfp);
  
  add_break(dfp);
  return;

}

# line 1492 "dynashadow.dy"
void write_matrix_to_special_max_func(DYNFILE * dfp,GenericMatrix * gm)
{
  register int i;
  register int j;
  FuncInfo * fi;

  char * special_tag = "DC_SHADOW_SPECIAL";
  char * matrix_tag = "DC_SHADOW_MATRIX";

  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"max_matrix_to_special_%s",gm->name);

  start_function_FuncInfo(fi,dfp,"int max_matrix_to_special_%s(%s * mat,int i,int j,int state,int cscore,int * reti,int * retj,int * retstate,boolean * retspecial,int * cellscore)",gm->name,gm->name);

  expr(dfp,"int temp");

  
  expr(dfp,"*reti = (*retj) = (*retstate) = %s_READ_OFF_ERROR;",gm->name);


  add_break(dfp);
  
  expr(dfp,"if( j < 0 || j > mat->lenj)",gm->target->name,gm->target_len);
  startbrace(dfp);
  expr(dfp,"warn(\"In %s matrix to special read off - out of bounds on matrix [j is %%d in special]\",j);",gm->name);
  expr(dfp,"return -1;");
  closebrace(dfp);
  
  add_break(dfp);

  expr(dfp,"switch(state)");
  
  startbrace_tag(dfp,"Switch state ");
  for(i=0;i<gm->len;i++) {
    auto CellState * cell;
    cell = gm->state[i];
    expr(dfp,"case %s : ",cell->name);
    startcase(dfp);
    for(j=cell->len-1;j>=0;j--) {
      if( cell->source[j]->isspecial == FALSE) {
	add_block_comment(dfp,"Source %s is not a special, should not get here!",cell->source[j]->state_source);
	continue;
      }

      expr(dfp,"temp = cscore - (%s) -  (%s)",cell->source[j]->calc_expr, 
	   cell->calc_expr == NULL ? "0" : cell->calc_expr);
      
      if( cell->source[j]->isspecial == TRUE)
	expr(dfp,"if( temp == %s_%s(mat,i - %d,j - %d,%s) )",gm->name,special_tag,cell->source[j]->offi,
	     cell->source[j]->offj,cell->source[j]->state_source);
      else	expr(dfp,"if( temp == %s_%s(mat,i - %d,j - %d,%s) )",gm->name,matrix_tag,cell->source[j]->offi,
		     cell->source[j]->offj,cell->source[j]->state_source);
      
      startbrace(dfp);
      expr(dfp,"*reti = i - %d;",cell->source[j]->offi);
      expr(dfp,"*retj = j - %d;",cell->source[j]->offj);
      expr(dfp,"*retstate = %s;",cell->source[j]->state_source);
      if( cell->source[j]->isspecial == TRUE)
	expr(dfp,"*retspecial = TRUE;");
      else expr(dfp,"*retspecial = FALSE;");
      
      /*** if they want the score, calc the difference in the number and give it back ***/
      expr(dfp,"if( cellscore != NULL)");
      startbrace(dfp);

      
      expr(dfp,"*cellscore = cscore - %s_%s(mat,i-%d,j-%d,%s)",gm->name,
	   cell->source[j]->isspecial == TRUE ? special_tag : matrix_tag,cell->source[j]->offi,
	   cell->source[j]->offj,cell->source[j]->state_source); 
      closebrace(dfp);
      
      
      expr(dfp,"return %s_%s(mat,i - %d,j - %d,%s) ",gm->name,matrix_tag,cell->source[j]->offi,
	   cell->source[j]->offj,cell->source[j]->state_source); 
      closebrace(dfp);
    }
    expr(dfp,"warn(\"Major problem (!) - in %s matrix to special read off, position %%d,%%d state %%d no source found!\",i,j,state);",gm->name);
    expr(dfp,"return (-1);");
    closecase(dfp);
  }
  expr(dfp,"default:");
  startcase(dfp);
  expr(dfp,"warn(\"Major problem (!) - in %s read off, position %%d,%%d state %%d no source found!\",i,j,state);",gm->name);
  expr(dfp,"return (-1);");
  closecase(dfp);
  
  
  closebrace(dfp);

  add_break(dfp);
  close_function(dfp);
  add_break(dfp);
}
  

# line 1586 "dynashadow.dy"
void write_special_strip_max_func(DYNFILE * dfp,GenericMatrix * gm)
{
  register int i;
  register int j;
  char * matrix_tag  = "DC_SHADOW_MATRIX";
  char * special_tag = "DC_SHADOW_SPECIAL";

  FuncInfo * fi;
  
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"max_special_strip_%s",gm->name);
  add_line_to_Ftext(fi->ft,"A pretty intense internal function. Deals with read-off only in specials");




  start_function_FuncInfo(fi,dfp,"int max_special_strip_%s(%s * mat,int i,int j,int state,boolean isspecial,int * reti,int * retj,int * retstate,boolean * retspecial,int * cellscore)",gm->name,gm->name);
  
  expr(dfp,"int temp;");
  expr(dfp,"int cscore;");
  
  add_break(dfp);

  
  
  expr(dfp,"*reti = (*retj) = (*retstate) = %s_READ_OFF_ERROR;",gm->name);


  expr(dfp,"if( isspecial == FALSE )");
  startbrace(dfp);
  warn_expr(dfp,"In special strip max function for %s, got a non special start point. Problem! (bad!)",gm->name);
  expr(dfp,"return (-1);");
  closebrace(dfp);

  
  add_break(dfp);
  
  expr(dfp,"if( j < 0 || j > mat->%s->%s)",gm->target->name,gm->target_len);
  startbrace(dfp);
  expr(dfp,"warn(\"In %s matrix special read off - out of bounds on matrix [j is %%d in special]\",j);",gm->name);
  expr(dfp,"return -1;");
  closebrace(dfp);
  
  add_break(dfp);
  
  expr(dfp,"cscore = %s_%s(mat,i,j,state);",gm->name,special_tag);
  
  
  expr(dfp,"switch(state)");
  startbrace_tag(dfp,"switch on special states");
  
  for(i=0;i<gm->spec_len;i++) {
    auto CellState * cell;
    cell = gm->special[i];
    expr(dfp,"case %s : ",cell->name);
    startcase(dfp);


    /*** ok, we are only interested in specials to specials, 
      otherwise we should not be here ***/



    for(j=cell->len-1;j>=0;j--) {
      
      if( cell->source[j]->isspecial == FALSE ) {
	add_block_comment(dfp,"Source %s is not a special",cell->source[j]->state_source);
	continue;

      }
      

      else {
	add_block_comment(dfp,"source %s is a special",cell->source[j]->state_source);

	expr(dfp,"temp = cscore - (%s) - (%s)",cell->source[j]->calc_expr,
	     cell->calc_expr == NULL ? "0" : cell->calc_expr);
      

	expr(dfp,"if( temp == %s_%s(mat,i - %d,j - %d,%s) )",gm->name,special_tag,cell->source[j]->offi,
	       cell->source[j]->offj,cell->source[j]->state_source);
          

	startbrace(dfp);
	expr(dfp,"*reti = i - %d;",cell->source[j]->offi);
	expr(dfp,"*retj = j - %d;",cell->source[j]->offj);
	expr(dfp,"*retstate = %s;",cell->source[j]->state_source);
	
	if( cell->source[j]->isspecial == TRUE)
	  expr(dfp,"*retspecial = TRUE;");
	else	expr(dfp,"*retspecial = FALSE;");

	/*** if they want the score, calc the difference in the number and give it back ***/
	expr(dfp,"if( cellscore != NULL)");
	startbrace(dfp);
	expr(dfp,"*cellscore = cscore - %s_%s(mat,i-%d,j-%d,%s)",gm->name,special_tag,cell->source[j]->offi,
	     cell->source[j]->offj,cell->source[j]->state_source); 
	closebrace(dfp);
      
	expr(dfp,"return %s_%s(mat,i - %d,j - %d,%s) ",gm->name,matrix_tag,cell->source[j]->offi,
	     cell->source[j]->offj,cell->source[j]->state_source); 
	closebrace(dfp);
      
      }
    }
    closecase(dfp);
  }
  
  expr(dfp,"default:");
  startcase(dfp);
  expr(dfp,"warn(\"Major problem (!) - in %s special strip read off, position %%d,%%d state %%d no source found  dropped into default on source switch!\",i,j,state);",gm->name);
  expr(dfp,"return (-1);");
  closecase(dfp);
  
  closebrace(dfp);
  
  close_function(dfp);
  
  add_break(dfp);
}



# line 1708 "dynashadow.dy"
void write_shadow_dc_macros(DYNFILE * dfp,GenericMatrix * gm)
{
  int steve_space;

  macro(dfp,"#define %s_HIDDEN_MATRIX(thismatrix,i,j,state) (thismatrix->basematrix->matrix[(j-hiddenj+%d)][(i+%d)*%d+state])",gm->name,gm->window_j,gm->window_i,gm->len);

  macro(dfp,"#define %s_DC_SHADOW_MATRIX(thismatrix,i,j,state) (thismatrix->basematrix->matrix[((j+%d)*8) %% %d][(i+%d)*%d+state])",gm->name,gm->window_j+1,(gm->window_j+1)*8,gm->window_i,gm->len);

  macro(dfp,"#define %s_HIDDEN_SPECIAL(thismatrix,i,j,state) (thismatrix->basematrix->specmatrix[state][(j+%d)])",gm->name,gm->window_j);

  macro(dfp,"#define %s_DC_SHADOW_SPECIAL(thismatrix,i,j,state) (thismatrix->basematrix->specmatrix[state*8][(j+%d)])",gm->name,gm->window_j);
  macro(dfp,"#define %s_DC_SHADOW_MATRIX_SP(thismatrix,i,j,state,shadow) (thismatrix->basematrix->matrix[((((j+%d)*8)+(shadow+1)) %% %d)][(i+%d)*%d + state])",gm->name,gm->window_j+1,(gm->window_j+1)*8,gm->window_i,gm->len);
  macro(dfp,"#define %s_DC_SHADOW_SPECIAL_SP(thismatrix,i,j,state,shadow) (thismatrix->basematrix->specmatrix[state*8 +shadow+1][(j+%d)])",gm->name,gm->window_j);

  /* we need 8 shadow pointers per matrix cell; one for the score 7 for different shadow implementations*/
  steve_space = gm->len * 8;

  macro(dfp,"#define %s_DC_OPT_SHADOW_MATRIX(thismatrix,i,j,state) (score_pointers[(((j+%d)%% %d) * (leni+1) * %d) + ((i+%d) * %d) + (state)])",gm->name,gm->window_j,gm->window_j,gm->len,gm->window_i,gm->len);

  /* the +1 is because + 0 is the score */
  macro(dfp,"#define %s_DC_OPT_SHADOW_MATRIX_SP(thismatrix,i,j,state,shadow) (shadow_pointers[(((j+%d)%% %d) * (leni+1) * %d) + ((i+%d) * %d) + (state * %d) + shadow+1])",gm->name,gm->window_j,gm->window_j,steve_space,gm->window_i,steve_space,8);


  /* currently for the optimised shadow we use the same special matrix layout */
  macro(dfp,"#define %s_DC_OPT_SHADOW_SPECIAL(thismatrix,i,j,state) (thismatrix->basematrix->specmatrix[state*8][(j+%d)])",gm->name,gm->window_j);
  macro(dfp,"#define %s_DC_OPT_SHADOW_SPECIAL_SP(thismatrix,i,j,state,shadow) (thismatrix->basematrix->specmatrix[state*8 +shadow+1][(j+%d)])",gm->name,gm->window_j);

  
}


/* Function:  write_shadow_dc_alloc(dfp,gm)
 *
 * Descrip:    Defunct (I think)
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:         gm [UNKN ] Undocumented argument [GenericMatrix *]
 *
 */
# line 1742 "dynashadow.dy"
void write_shadow_dc_alloc(DYNFILE * dfp,GenericMatrix * gm)
{
  FuncInfo * fi;
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"do_dc_sin_%s",gm->name);

  start_function(dfp,"%s * shadow_dc_alloc_%s(int leni,int lenj)",gm->name,gm->name);
  expr(dfp,"register int i;");
  expr(dfp,"register %s * out",gm->name);

  add_break(dfp);
  add_block_comment(dfp,"Have to have enough memory to hold i,j,state triple *3 for each state");
  add_block_comment(dfp,"This memory will also be used for the explicit memory use at end of r");
  add_break(dfp);
  add_block_comment(dfp,"This means we need (window_j+1)*8 top line pointers by cell number * leni");
  add_break(dfp);

  expr(dfp,"out = %s_alloc()",gm->name);
  expr(dfp,"if(out == NULL)");
  hang_expr(dfp,"return NULL;");
  add_end_comment(dfp,"warning already issued");

  add_break(dfp);

  expr(dfp,"out->basematrix = BaseMatrix_alloc_matrix_and_specials(%d,(leni + %d) * %d,%d,lenj+%d)",(gm->window_j+1)*8,gm->window_i,gm->len,gm->spec_len*8,gm->window_j);
  expr(dfp,"if(out->basematrix == NULL)");
  startbrace(dfp);
  expr(dfp,"warn(\"In shadow_dc_alloc_%s unable to allocate basematrix with main matrix %d by %%d ints\",(leni+%d)*%d);",gm->name,(gm->window_j+1)*8,gm->window_i,gm->len);
  expr(dfp,"free_%s(out)",gm->name);
  expr(dfp,"return NULL;");
  closebrace(dfp);

  add_break(dfp);

  expr(dfp,"return out;");

  close_function(dfp);
  add_break(dfp);
}




 /***
   Start End Stuff 
   ***/


# line 1789 "dynashadow.dy"
void write_start_end_init(DYNFILE * dfp,GenericMatrix * gm)
{
  register int s;
  register int i;
  FuncInfo * fi;

  for(i=0;i<gm->spec_len;i++)
    if( gm->special[i]->is_start == TRUE)
      break;

  if( i >= gm->spec_len ) {
    log_full_error(WARNING,0,"Cannot build start/end init function due to lack of start state");
    return;
  }

  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"init_start_end_linear_%s",gm->name);

  start_function_FuncInfo(fi,dfp,"void init_start_end_linear_%s(%s * mat)",gm->name,gm->name);
  expr(dfp,"register int i;");
  expr(dfp,"register int j;");


  expr(dfp,"for(j=0;j<%d;j++)",gm->window_j+2);
  startbrace(dfp);
  expr(dfp,"for(i=(-%d);i<mat->%s->%s;i++)",gm->window_i,gm->query->name,gm->query_len);
  startbrace(dfp);
  for(s=0;s<gm->len;s++) {
    expr(dfp,"%s_DC_SHADOW_MATRIX(mat,i,j,%s) = %s;",gm->name,gm->state[s]->name,gm->state[s]->def_score);
    expr(dfp,"%s_DC_SHADOW_MATRIX_SP(mat,i,j,%s,0) = (-1);",gm->name,gm->state[s]->name);
  }
  closebrace(dfp);
  closebrace(dfp);

  add_break(dfp);

  expr(dfp,"for(j=(-%d);j<mat->%s->%s;j++)",gm->window_j,gm->target->name,gm->target_len);
  startbrace(dfp);
  for(s=0;s<gm->spec_len;s++) {
    expr(dfp,"%s_DC_SHADOW_SPECIAL(mat,0,j,%s) = %s;",gm->name,gm->special[s]->name,gm->special[s]->def_score);
    if( s == i ) /** is start **/
      expr(dfp,"%s_DC_SHADOW_SPECIAL_SP(mat,0,j,%s,0) = j;",gm->name,gm->special[s]->name);
    else 
      expr(dfp,"%s_DC_SHADOW_SPECIAL_SP(mat,0,j,%s,0) = (-1);",gm->name,gm->special[s]->name);
  }
  closebrace(dfp);

  add_break(dfp);
  expr(dfp,"return");
  close_function(dfp);
  add_break(dfp);
}


# line 1842 "dynashadow.dy"
void write_start_end_find_end(DYNFILE * dfp,GenericMatrix * gm)
{

  register int i;

  ArgInfo * ai;
  FuncInfo * fi;
  
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"start_end_find_end_%s",gm->name);
  add_line_to_Ftext(fi->ft,"First function used to find end of the best path in the special state !end");
  
  ai = ArgInfo_in_FuncInfo_from_varstr(fi,"mat");
  ai->desc = stringalloc("Matrix in small mode");

  ai = ArgInfo_in_FuncInfo_from_varstr(fi,"endj");
  ai->desc = stringalloc("position of end in j (meaningless in i)");
  ai->argtype = ARGTYPE_WRITE;
  
  for(i=0;i<gm->spec_len;i++)
    if( gm->special[i]->is_end == TRUE)
      break;

  if( i >= gm->spec_len ) {
    log_full_error(WARNING,0,"Cannot build start/end find end function due to lack of end state. YIKES!");
    return;
  }
  

  start_function_FuncInfo(fi,dfp,"int start_end_find_end_%s(%s * mat,int * endj)",gm->name,gm->name);
	
  expr(dfp,"register int j");
  expr(dfp,"register int max;");
  expr(dfp,"register int maxj");
  
  add_break(dfp);
  
  expr(dfp,"max = %s_DC_SHADOW_SPECIAL(mat,0,mat->%s->%s-1,%s);",gm->name,gm->target->name,gm->target_len,gm->special[i]->name);
  expr(dfp,"maxj = mat->%s->%s-1",gm->target->name,gm->target_len);
  expr(dfp,"for(j= mat->%s->%s-2 ;j >= 0 ;j--)",gm->target->name,gm->target_len);
  
  startbrace(dfp);
  expr(dfp,"if( %s_DC_SHADOW_SPECIAL(mat,0,j,%s) > max )",gm->name,gm->special[i]->name);
  startbrace(dfp);
  expr(dfp,"max = %s_DC_SHADOW_SPECIAL(mat,0,j,%s);",gm->name,gm->special[i]->name);
  expr(dfp,"maxj = j;");
  closebrace(dfp);
  closebrace(dfp);
  
  add_break(dfp);
  

  expr(dfp,"if( endj != NULL)");
  hang_expr(dfp,"*endj = maxj;");
  
  add_break(dfp);
  
  expr(dfp,"return max");
  
  close_function(dfp);
  
  add_break(dfp);  
}
	

/* Function:  heavy_optimised_shadow_GenericMatrix(dfp,gm)
 *
 * Descrip:    heavily optimised start end calc function
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:         gm [UNKN ] Undocumented argument [GenericMatrix *]
 *
 */
# line 1909 "dynashadow.dy"
void heavy_optimised_shadow_GenericMatrix(DYNFILE * dfp,GenericMatrix * gm)
{
  int i;
  int j;
  int k;
  FuncInfo * fi;

  char * matrixtag = "DC_OPT_SHADOW_MATRIX";
  char * shadow_main_tag = "DC_OPT_SHADOW_MATRIX_SP";
  char * shadow_special_tag = "DC_OPT_SHADOW_SPECIAL_SP";
  char * specialtag = "DC_OPT_SHADOW_SPECIAL";


  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"dc_heavy_optimised_start_end_calc_%s",gm->name);
  add_line_to_Ftext(fi->ft,"Calculates special strip, leaving start/end/score points in shadow matrix");
  add_line_to_Ftext(fi->ft,"Works off specially laid out memory from steve searle");
	

  start_function_FuncInfo(fi,dfp,"boolean dc_heavy_optimised_start_end_calc_%s(%s *mat,DPEnvelope * dpenv)",gm->name,gm->name);

  expr(dfp,"int i;");
  expr(dfp,"int j;");
  expr(dfp,"int k;");


  expr(dfp,"int leni;");
  expr(dfp,"int lenj;");
  expr(dfp,"int localshadow[7]");
  expr(dfp,"long int total");
  expr(dfp,"long int num=0");
  expr(dfp,"int * score_pointers");
  expr(dfp,"int * shadow_pointers");


  for(i=0;i<gm->len;i++) {
    expr(dfp,"int score_%s;",gm->state[i]->name);
    expr(dfp,"int temp_%s;",gm->state[i]->name);
    expr(dfp,"int * localsp_%s;",gm->state[i]->name);
    for(j=0;j<gm->state[i]->len;j++) {
      expr(dfp,"int score_%s_%d;",gm->state[i]->name,j);
    }
  }

  add_break(dfp);

  expr(dfp,"leni = mat->%s->%s;",gm->query->name,gm->query_len);
  expr(dfp,"lenj = mat->%s->%s;",gm->target->name,gm->target_len);

  add_break(dfp);

  expr(dfp,"score_pointers = (int *) calloc (%d * (leni + %d) * %d,sizeof(int));",gm->window_j,gm->window_i,gm->len);
  expr(dfp,"shadow_pointers = (int *) calloc (%d * (leni + %d) * %d * 8,sizeof(int));",gm->window_j,gm->window_i,gm->len);


  add_break(dfp);

  expr(dfp,"for(j=0;j<lenj;j++)");
  startbrace_tag(dfp,"for each j strip");
  expr(dfp,"for(i=0;i<leni;i++)");
  startbrace_tag(dfp,"for each i position in strip");
  expr(dfp,"num++");


  expr(dfp,"if( dpenv != NULL && is_in_DPEnvelope(dpenv,i,j) == FALSE )");
  startbrace_tag(dfp,"Is not in envelope");

  for(i=0;i<gm->len;i++) {
    expr(dfp,"%s_DC_OPT_SHADOW_MATRIX(mat,i,j,%s) = NEGI",gm->name,gm->state[i]->name);
  }
  expr(dfp,"continue;");
  closebrace(dfp);


  expr(dfp,"if( num%%1000 == 0)");
  hang_expr(dfp,"log_full_error(REPORT,0,\"%%6d Cells done [%%2d%%%%%%%%]\",num,num*100/total);");

  add_break(dfp);


  /* main block */

  add_block_comment(dfp,"Retrieve all the calculations first, and put into local variables");
  
  for(i=0;i<gm->len;i++) {
    for(j=0;j<gm->state[i]->len;j++) {
      expr(dfp,"score_%s_%d = %s_%s(mat,i-%d,j-%d,%s) + %s + (%s)",
	   gm->state[i]->name,j,
	   gm->name,
	   gm->state[i]->source[j]->isspecial == TRUE ? specialtag : matrixtag, 
	   gm->state[i]->source[j]->offi,gm->state[i]->source[j]->offj,
	   gm->state[i]->source[j]->state_source,
	   gm->state[i]->source[j]->calc_expr,
	   gm->state[i]->calc_expr == NULL ? "0" : gm->state[i]->calc_expr
	   );
    }
  }
  
  add_break(dfp);

  add_block_comment(dfp,"now do the if/then/else store");

  for(i=0;i<gm->len;i++) {
    add_block_comment(dfp,"handling state %s",gm->state[i]->name);

    expr(dfp,"score_%s   = score_%s_0;",gm->state[i]->name,gm->state[i]->name);
    expr(dfp,"localsp_%s = &(%s_%s(mat,i - %d,j - %d,%s,0));",gm->state[i]->name,
	 gm->name,gm->state[i]->source[0]->isspecial == TRUE ? shadow_special_tag : shadow_main_tag,
	 gm->state[i]->source[0]->offi,gm->state[i]->source[0]->offj,gm->state[i]->source[0]->state_source);

    for(j=1;j<gm->state[i]->len;j++) {
      expr(dfp,"if( score_%s_%d > score_%s)",gm->state[i]->name,j,gm->state[i]->name);
      startbrace(dfp);
      expr(dfp,"score_%s = score_%s_%d;",gm->state[i]->name,gm->state[i]->name,j);
      if( gm->state[i]->source[j]->isspecial == TRUE ) {
	add_block_comment(dfp,"This source is a special, push top shadow pointers");
	expr(dfp,"localshadow[0] = i;");
	expr(dfp,"localshadow[1] = j;");
	expr(dfp,"localshadow[2] = %s;",gm->state[i]->name);
	expr(dfp,"localshadow[3] = -1;");
	expr(dfp,"localshadow[4] = -1;");
	expr(dfp,"localshadow[5] = -1;");
	expr(dfp,"localshadow[6] = score_%s;",gm->state[i]->name);
	expr(dfp,"localsp_%s = localshadow;",gm->state[i]->name);
      } else {
	expr(dfp,"localsp_%s = &(%s_%s(mat,i -%d,j - %d,%s,0));",gm->state[i]->name,
	     gm->name,shadow_main_tag,
	     gm->state[i]->source[j]->offi,gm->state[i]->source[j]->offj,gm->state[i]->source[j]->state_source);
      }
      closebrace(dfp);

    } /* end of loop over all sources */


    expr(dfp,"%s_%s(mat,i,j,%s) = score_%s",gm->name,matrixtag,gm->state[i]->name,gm->state[i]->name);
    expr(dfp,"for(k=0;k<7;k++)");
    hang_expr(dfp,"%s_%s(mat,i,j,%s,k) = localsp_%s[k]",gm->name,shadow_main_tag,gm->state[i]->name,gm->state[i]->name);

    for(j=0;j<gm->spec_len;j++) {
      auto CellState * specstate;
      specstate = gm->special[j];
      
      
      for(k=0;k<specstate->len;k++) {
	if( strcmp(specstate->source[k]->state_source,gm->state[i]->name) == 0) {
	  /********************************/
	  /* is a special source!         */
	  /********************************/
	  add_break(dfp);
	  add_block_comment(dfp,"state %s is a source for special %s",gm->state[i]->name,specstate->name);
	  expr(dfp,"temp_%s = score_%s + (%s) + (%s) ",gm->state[i]->name,gm->state[i]->name,specstate->source[k]->calc_expr,specstate->calc_expr == NULL ? "0" : specstate->calc_expr );
	  expr(dfp,"if( temp_%s > %s_%s(mat,i,j,%s) ) ",gm->state[i]->name,gm->name,specialtag,specstate->name);
	  startbrace(dfp);
	  expr(dfp,"%s_%s(mat,i,j,%s) = temp_%s",gm->name,specialtag,specstate->name,gm->state[i]->name);
	  
	  add_block_comment(dfp,"Have to push only bottem half of system here");
	  expr(dfp,"for(k=0;k<3;k++)");
	  hang_expr(dfp,"%s_%s(mat,i,j,%s,k) = %s_%s(mat,i,j,%s,k);",
		    gm->name,shadow_special_tag,
		    specstate->name, gm->name,
		    specstate->source[k]->isspecial == TRUE ? shadow_special_tag : shadow_main_tag,
		    gm->state[i]->name);
	  
	  /*** sort of hacky ***/
	  expr(dfp,"%s_%s(mat,i,j,%s,6) = temp_%s;",
	       gm->name,shadow_special_tag,
	       specstate->name, gm->state[i]->name);



	  expr(dfp,"%s_%s(mat,i,j,%s,3) = i;",gm->name,shadow_special_tag,specstate->name);
	  expr(dfp,"%s_%s(mat,i,j,%s,4) = j;",gm->name,shadow_special_tag,specstate->name);
	  expr(dfp,"%s_%s(mat,i,j,%s,5) = %s;",gm->name,shadow_special_tag,specstate->name,gm->state[i]->name);
	  
	  closebrace(dfp);
	}
      }
    } /* end of loop over all special states */

    add_block_comment(dfp,"Finished if/then/else loop for state %s",gm->state[i]->name);

  } /* end of loop over states */

  add_break(dfp);

  closebrace(dfp);
  
  if( gm->specialtospecial == TRUE ) 
    write_special_shadow_block(dfp,gm,"DC_OPT_SHADOW_MATRIX","mat","DC_OPT_SHADOW_SPECIAL","DC_OPT_SHADOW_MATRIX_SP","DC_OPT_SHADOW_SPECIAL_SP",7,TRUE,0);

  closebrace(dfp);

  add_break;

  expr(dfp,"free(score_pointers);");
  expr(dfp,"free(shadow_pointers);");

  expr(dfp,"return TRUE");

  close_function(dfp);

  add_break(dfp);

}


/* Function:  optimised_shadow_GenericMatrix(dfp,*gm)
 *
 * Descrip:    Makes the optimised shadow matrix routine,
 *             worked out by Steve Searle - memory access
 *             is put into one array so that the routine
 *             is faster
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:        *gm [UNKN ] Undocumented argument [const GenericMatrix]
 *
 */
# line 2120 "dynashadow.dy"
void optimised_shadow_GenericMatrix(DYNFILE * dfp,const GenericMatrix *gm)
{
  int i;
  FuncInfo * fi;

  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"dc_optimised_start_end_calc_%s",gm->name);
  add_line_to_Ftext(fi->ft,"Calculates special strip, leaving start/end/score points in shadow matrix");
  add_line_to_Ftext(fi->ft,"Works off specially laid out memory from steve searle");
	

  start_function_FuncInfo(fi,dfp,"boolean dc_optimised_start_end_calc_%s(%s *mat,DPEnvelope * dpenv)",gm->name,gm->name);

  expr(dfp,"int i;");
  expr(dfp,"int j;");
  expr(dfp,"int k;");
  expr(dfp,"int score;");
  expr(dfp,"int temp;");
  expr(dfp,"int leni;");
  expr(dfp,"int lenj;");
  expr(dfp,"int localshadow[7]");
  expr(dfp,"long int total");
  expr(dfp,"long int num=0");
  expr(dfp,"int * score_pointers");
  expr(dfp,"int * shadow_pointers");
  expr(dfp,"int * localsp;");
  
  expr(dfp,"leni = mat->%s->%s;",gm->query->name,gm->query_len);
  expr(dfp,"lenj = mat->%s->%s;",gm->target->name,gm->target_len);
  expr(dfp,"total = leni * lenj;");

  add_break(dfp);

  expr(dfp,"score_pointers = (int *) calloc (%d * (leni + %d) * %d,sizeof(int));",gm->window_j,gm->window_i,gm->len);
  expr(dfp,"shadow_pointers = (int *) calloc (%d * (leni + %d) * %d * 8,sizeof(int));",gm->window_j,gm->window_i,gm->len);


  add_break(dfp);

  expr(dfp,"for(j=0;j<lenj;j++)");
  startbrace_tag(dfp,"for each j strip");
  expr(dfp,"for(i=0;i<leni;i++)");
  startbrace_tag(dfp,"for each i position in strip");
  expr(dfp,"num++");


  expr(dfp,"if( dpenv != NULL && is_in_DPEnvelope(dpenv,i,j) == FALSE )");
  startbrace_tag(dfp,"Is not in envelope");

  for(i=0;i<gm->len;i++) {
    expr(dfp,"%s_DC_OPT_SHADOW_MATRIX(mat,i,j,%s) = NEGI",gm->name,gm->state[i]->name);
  }
  expr(dfp,"continue;");
  closebrace(dfp);


  expr(dfp,"if( num%%1000 == 0)");
  hang_expr(dfp,"log_full_error(REPORT,0,\"%%6d Cells done [%%2d%%%%%%%%]\",num,num*100/total);");

  add_break(dfp);

  write_main_shadow_block(dfp,gm,"DC_OPT_SHADOW_MATRIX","mat","DC_OPT_SHADOW_SPECIAL","DC_OPT_SHADOW_MATRIX_SP","DC_OPT_SHADOW_SPECIAL_SP",7,TRUE,TRUE,0,TRUE);

  add_break(dfp);

  closebrace(dfp);

  if( gm->specialtospecial == TRUE ) 
    write_special_shadow_block(dfp,gm,"DC_OPT_SHADOW_MATRIX","mat","DC_OPT_SHADOW_SPECIAL","DC_OPT_SHADOW_MATRIX_SP","DC_OPT_SHADOW_SPECIAL_SP",7,TRUE,0);

  closebrace(dfp);

  add_break;

  expr(dfp,"free(score_pointers);");
  expr(dfp,"free(shadow_pointers);");

  /*  expr(dfp,"free(mat->basematrix->optimised_shadow);"); */

  expr(dfp,"return TRUE");

  close_function(dfp);

  add_break(dfp);
  
}



# line 2208 "dynashadow.dy"
void write_start_end_build(DYNFILE * dfp,GenericMatrix * gm)
{
  int i;
  FuncInfo * fi;
  
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"dc_start_end_calculate_%s",gm->name);
  add_line_to_Ftext(fi->ft,"Calculates special strip, leaving start/end/score points in the shadow matrix ");
  add_line_to_Ftext(fi->ft,"One tricky thing is that we need to add score-independent calcs in the states");
  add_line_to_Ftext(fi->ft,"As we have to evaluate them then. This is not ideally implemented therefore ");
  add_line_to_Ftext(fi->ft,"In fact it is *definitely* not ideal. Will have to do for now");



  start_function_FuncInfo(fi,dfp,"boolean dc_start_end_calculate_%s(%s * mat,DPEnvelope * dpenv)",gm->name,gm->name);
  expr(dfp,"int i;");
  expr(dfp,"int j;");
  expr(dfp,"int k;");
  expr(dfp,"int score;");
  expr(dfp,"int temp;");
  expr(dfp,"int leni;");
  expr(dfp,"int lenj;");
  expr(dfp,"int localshadow[7]");
  expr(dfp,"long int total");
  expr(dfp,"long int num=0");


  add_break(dfp);
  expr(dfp,"init_start_end_linear_%s(mat)",gm->name);
  add_break(dfp);

  expr(dfp,"leni = mat->%s->%s;",gm->query->name,gm->query_len);
  expr(dfp,"lenj = mat->%s->%s;",gm->target->name,gm->target_len);
  expr(dfp,"total = leni * lenj;");

  add_break(dfp);

  expr(dfp,"for(j=0;j<lenj;j++)");
  startbrace_tag(dfp,"for each j strip");
  expr(dfp,"for(i=0;i<leni;i++)");
  startbrace_tag(dfp,"for each i position in strip");
  expr(dfp,"num++");


  expr(dfp,"if( dpenv != NULL && is_in_DPEnvelope(dpenv,i,j) == FALSE )");
  startbrace_tag(dfp,"Is not in envelope");

  for(i=0;i<gm->len;i++) {
    expr(dfp,"%s_DC_SHADOW_MATRIX(mat,i,j,%s) = NEGI",gm->name,gm->state[i]->name);
  }
  expr(dfp,"continue;");
  closebrace(dfp);


  expr(dfp,"if( num%%1000 == 0)");
  hang_expr(dfp,"log_full_error(REPORT,0,\"%%6d Cells done [%%2d%%%%%%%%]\",num,num*100/total);");

  add_break(dfp);

  write_main_shadow_block(dfp,gm,"DC_SHADOW_MATRIX","mat","DC_SHADOW_SPECIAL","DC_SHADOW_MATRIX_SP","DC_SHADOW_SPECIAL_SP",7,TRUE,TRUE,0,FALSE);

  add_break(dfp);

  closebrace(dfp);

  if( gm->specialtospecial == TRUE ) 
    write_special_shadow_block(dfp,gm,"DC_SHADOW_MATRIX","mat","DC_SHADOW_SPECIAL","DC_SHADOW_MATRIX_SP","DC_SHADOW_SPECIAL_SP",7,TRUE,0);

  closebrace(dfp);

  expr(dfp,"return TRUE");

  close_function(dfp);

  add_break(dfp);
}



# line 2286 "dynashadow.dy"
void write_start_end_macros(DYNFILE * dfp,GenericMatrix * gm)
{
  macro(dfp,"#define %s_SE_SHADOW_MATRIX(thismatrix,i,j,state) (thismatrix->basematrix->matrix[((j+%d)*2) %% %d][(i+%d)*%d+state])",gm->name,gm->window_j+1,(gm->window_j+1)*2,gm->window_i,gm->len);
  macro(dfp,"#define %s_SE_SHADOW_SPECIAL(thismatrix,i,j,state) (thismatrix->basematrix->specmatrix[state*2][(j+%d)])",gm->name,gm->window_j);
  macro(dfp,"#define %s_SE_SHADOW_MATRIX_SP(thismatrix,i,j,state,shadow) (thismatrix->basematrix->matrix[( (((j+%d)*2)+(shadow+1)) %% %d)][(i+%d)*%d + state])",gm->name,gm->window_j+1,(gm->window_j+1)*2,gm->window_i,gm->len);
  macro(dfp,"#define %s_SE_SHADOW_SPECIAL_SP(thismatrix,i,j,state,shadow) (thismatrix->basematrix->specmatrix[state*2 +(shadow+1)][(j+%d)])",gm->name,gm->window_j);

  add_break(dfp);
}


# line 2297 "dynashadow.dy"
void write_shadow_start_end_alloc(DYNFILE * dfp,GenericMatrix * gm)
{
  start_function(dfp,"%s * shadow_se_alloc_%s(int leni,int lenj)",gm->name,gm->name);
  expr(dfp,"register %s * out",gm->name);

  add_break(dfp);
  add_block_comment(dfp,"Have to have enough memory to hold start for each state");
  add_break(dfp);
  add_block_comment(dfp,"This means we need (window_j+1)*2 top line pointers by cell number * leni");
  add_break(dfp);

  expr(dfp,"out = %s_alloc()",gm->name);
  expr(dfp,"if(out == NULL)");
  hang_expr(dfp,"return NULL;");
  add_end_comment(dfp,"warning already issued");

  add_break(dfp);

  expr(dfp,"out->basematrix = BaseMatrix_alloc_matrix_and_specials(%d,(leni + %d) * %d,%d,lenj+%d)",(gm->window_j+1)*2,gm->window_i,gm->len,gm->spec_len*2,gm->window_j);
  expr(dfp,"if(out->basematrix == NULL)");
  startbrace(dfp);
  expr(dfp,"warn(\"In shadow_dc_alloc_%s unable to allocate basematrix with main matrix %d by %%d ints\",(leni+%d)*%d);",gm->name,(gm->window_j+1)*2,gm->window_i,gm->len);
  expr(dfp,"free_%s(out)",gm->name);
  expr(dfp,"return NULL;");
  closebrace(dfp);

  add_break(dfp);

  expr(dfp,"return out;");

  close_function(dfp);
  add_break(dfp);
}
  

/* Function:  write_main_shadow_block(dfp,gm,matrixtag,pointer_tag,specialtag,shadow_main_tag,shadow_special_tag,shadow_length,shadow_on_special,use_special,debug,use_shadow_pointer)
 *
 * Descrip:    The core inner loop for shadow methods. Pretty terrifying stuff in here.
 *
 *             Shadow is considered to be in the memory shadow_main_tag and shadow_special_tag
 *             and usually has the form MATRIX_TYPE_MAIN/SPECIAL_SP. 
 *
 *             shadow has positions 0-length-1, as defined by shadow_length. These are
 *             indexed by k.
 *
 *             Either shadow_on_special is false
 *             Some other routine has to place the shadow pointers. This routine just
 *             propagates the shadow pointers around
 *
 *             Or if shadow_on_special  is true
 *             This routines pushes when it leaves special and also pushes when it 
 *             enters special. This way, special states have the necessary information
 *             to know where in the main matrix they were made from. Means there must be 7 shadow
 *             positions.
 *
 *             In addition, as a complete mess, shadow on special needs to have score independent
 *             calcs added onto each movement, otherwise the scores are incorrect. So... this
 *             is not *IDEAL* in the slightest!!!
 *
 *
 * Arg:                       dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:                        gm [UNKN ] Undocumented argument [GenericMatrix *]
 * Arg:                 matrixtag [UNKN ] Undocumented argument [char *]
 * Arg:               pointer_tag [UNKN ] Undocumented argument [char *]
 * Arg:                specialtag [UNKN ] Undocumented argument [char *]
 * Arg:           shadow_main_tag [UNKN ] Undocumented argument [char *]
 * Arg:        shadow_special_tag [UNKN ] Undocumented argument [char *]
 * Arg:             shadow_length [UNKN ] Undocumented argument [int]
 * Arg:         shadow_on_special [UNKN ] Undocumented argument [boolean]
 * Arg:               use_special [UNKN ] Undocumented argument [boolean]
 * Arg:                     debug [UNKN ] Undocumented argument [int]
 * Arg:        use_shadow_pointer [UNKN ] Undocumented argument [int]
 *
 */
# line 2355 "dynashadow.dy"
void write_main_shadow_block(DYNFILE * dfp,GenericMatrix * gm,char * matrixtag,char * pointer_tag,char * specialtag,char * shadow_main_tag,char * shadow_special_tag,int shadow_length,boolean shadow_on_special,boolean use_special,int debug,int use_shadow_pointer)
{
  register int i;
  register int j;
  register int k;

  /*** this function assummes the following local variables

    int score
    int temp
    int k
    int localshadow[SHADOW_LENGTH]
    int * localsp
    ***/

  if( shadow_on_special == TRUE && shadow_length != 7)
    warn("In writing main shadow block [main matrix tag %s], attempting to push on specials, and yet shadow_length is %d",matrixtag,shadow_length);

  if( shadow_on_special == TRUE && use_special == FALSE ) {
    fatal("Look, you can't shaodw on special but not being using the specials... get otta here");
  }

  for(i=0;i<gm->len;i++) {
    auto CellState * state;
    state = gm->state[i];
    
    add_break(dfp);

    add_block_comment(dfp,"For state %s",state->name);
    add_block_comment(dfp,"setting first movement to score",state->name);
    
    /*********************************************************************/
    /* this line looks like                                              */
    /*   score = ProteinMatrix_EXPL_MATRIX(mat,i-1,j-1,MATCH) + xxxxx    */
    /*********************************************************************/

    if( shadow_on_special ) 
      expr(dfp,"score = %s_%s(%s,i-%d,j-%d,%s) + %s + (%s)",gm->name,matrixtag,pointer_tag,
	   state->source[0]->offi,state->source[0]->offj,state->source[0]->state_source,
	   state->source[0]->calc_expr,state->calc_expr == NULL ? "0" : state->calc_expr);
    else 
      expr(dfp,"score = %s_%s(%s,i-%d,j-%d,%s) + %s",gm->name,matrixtag,pointer_tag,
	   state->source[0]->offi,state->source[0]->offj,state->source[0]->state_source,
	   state->source[0]->calc_expr);
    

    /*** must also shift shadown positions ***/

    if( use_shadow_pointer == FALSE ) {
      add_block_comment(dfp,"shift first shadow numbers");

      expr(dfp,"for(k=0;k<%d;k++)",shadow_length);

      hang_expr(dfp,"localshadow[k] = %s_%s(mat,i - %d,j - %d,%s,k);",
		gm->name,state->source[0]->isspecial == TRUE ? shadow_special_tag : shadow_main_tag,
		state->source[0]->offi,state->source[0]->offj,state->source[0]->state_source);
    } else {
      add_block_comment(dfp,"assign local shadown pointer");
      expr(dfp,"localsp = &(%s_%s(mat,i - %d,j - %d,%s,0));",
	   gm->name,state->source[0]->isspecial == TRUE ? shadow_special_tag : shadow_main_tag,
	   state->source[0]->offi,state->source[0]->offj,state->source[0]->state_source);
    }


    /**** ok this is to stop underflow, but is v.v.v. hacky ****/
    /*** removing underflow hack 
    expr(dfp,"if(score < (-10000000) )");
    hang_expr(dfp,"score = (-10000000)");
    ****/
    
    
    /****************************************/
    /* now we do if then on score and temp  */
    /****************************************/
    
    for(j=1;j<state->len;j++)	{

      if( use_special == FALSE && state->source[j]->isspecial == TRUE )
	continue;



      add_block_comment(dfp,"From state %s to state %s",state->source[j]->state_source,
			state->name);	

      /*** ok, really nasty piece coming up: The expr is going to change wrt to things
	a) using special or not (which will require source independent score to be added)
	b) special source or not.

	Hence the nested if. Ugh.

	***/

      if( shadow_on_special ) {

	/** add in source-independent score **/

	if( state->source[j]->isspecial == TRUE )
	  expr(dfp,"temp = %s_%s(%s,i-%d,j-%d,%s) + %s + (%s)",gm->name,specialtag,pointer_tag,
	       state->source[j]->offi,state->source[j]->offj,state->source[j]->state_source,
	       state->source[j]->calc_expr,state->calc_expr == NULL ? "0" : state->calc_expr);
	else	expr(dfp,"temp = %s_%s(%s,i-%d,j-%d,%s) + %s +(%s)",gm->name,matrixtag,pointer_tag,
		     state->source[j]->offi,state->source[j]->offj,state->source[j]->state_source,
		     state->source[j]->calc_expr,state->calc_expr == NULL ? "0" : state->calc_expr);
      }
      else {

	if( state->source[j]->isspecial == TRUE )
	  expr(dfp,"temp = %s_%s(%s,i-%d,j-%d,%s) + %s",gm->name,specialtag,pointer_tag,
	       state->source[j]->offi,state->source[j]->offj,state->source[j]->state_source,
	       state->source[j]->calc_expr);
	else	expr(dfp,"temp = %s_%s(%s,i-%d,j-%d,%s) + %s",gm->name,matrixtag,pointer_tag,
		     state->source[j]->offi,state->source[j]->offj,state->source[j]->state_source,
		     state->source[j]->calc_expr);
      }


      /**** ok this is to stop underflow, but is v.v.v. hacky ****/
      /**** removing underflow hack
      expr(dfp,"if(score < (-10000000) )");
      hang_expr(dfp,"score = (-10000000)");
      ****/

      if( debug == 2 )
	expr(dfp,"fprintf(stderr,\"[%%4d,%%4d] Score of %s from %s gets %%d\\n\",i,j,temp)",state->name,state->source[j]->state_source );
				

      expr(dfp,"if( temp  > score )");
      startbrace(dfp);
      expr(dfp,"score = temp;");
      /** ok for shadow matrix should put things in here */

      if( shadow_on_special == TRUE && state->source[j]->isspecial == TRUE) {
	add_block_comment(dfp,"This state [%s] is a special for %s... push top shadow pointers here",state->source[j]->state_source,state->name);
	expr(dfp,"localshadow[0]= i;");
	expr(dfp,"localshadow[1]= j;");
	expr(dfp,"localshadow[2]= %s;",state->name);
	expr(dfp,"localshadow[3]= (-1);");
	expr(dfp,"localshadow[4]= (-1);");
	expr(dfp,"localshadow[5]= (-1);");
	expr(dfp,"localshadow[6]= score;");
	if( use_shadow_pointer == TRUE ) {
	  expr(dfp,"localsp = localshadow;");
	}
      }
      else {

	if( use_shadow_pointer == FALSE ) {
	  expr(dfp,"for(k=0;k<%d;k++)",shadow_length);
	  
	  hang_expr(dfp,"localshadow[k] = %s_%s(mat,i - %d,j - %d,%s,k);",
		    gm->name,state->source[0]->isspecial == TRUE ? shadow_special_tag : shadow_main_tag,
		    state->source[j]->offi,state->source[j]->offj,state->source[j]->state_source);

/*** now using local shadow system
	hang_expr(dfp,"%s_%s(%s,i,j,%s,k) = %s_%s(%s,i - %d,j - %d,%s,k);",
		  gm->name,shadow_main_tag,pointer_tag,state->name,
		  gm->name,state->source[j]->isspecial == TRUE ? shadow_special_tag : shadow_main_tag,pointer_tag,state->source[j]->offi,
		  state->source[j]->offj,state->source[j]->state_source);
***/
	} else {
	  add_block_comment(dfp,"assign local shadown pointer");
	  expr(dfp,"localsp = &(%s_%s(mat,i - %d,j - %d,%s,0));",
	       gm->name,state->source[j]->isspecial == TRUE ? shadow_special_tag : shadow_main_tag,
	       state->source[j]->offi,state->source[j]->offj,state->source[j]->state_source);
	}

      }


      /*** end if score > temp ***/
      closebrace(dfp);
    }
    
    /************************/
    /* finished blocks      */
    /* put in global calc   */
    /************************/
    add_break(dfp);
    add_block_comment(dfp,"Ok - finished max calculation for %s",state->name);
    add_block_comment(dfp,"Add any movement independant score and put away"); 
    
    if( shadow_on_special ) 
      add_block_comment(dfp,"Actually, already done inside scores");
    else if( state->calc_expr != NULL)
      expr(dfp," score += %s",state->calc_expr); 
    


    /***************************/			
    /* put away score          */
    /***************************/
    
    expr(dfp," %s_%s(%s,i,j,%s) = score;",gm->name,matrixtag,pointer_tag,state->name);


    /***************************/
    /* put away localshadow    */
    /***************************/

    if( use_shadow_pointer == FALSE ) {
      expr(dfp,"for(k=0;k<%d;k++)",shadow_length);
      hang_expr(dfp,"%s_%s(%s,i,j,%s,k) = localshadow[k];",gm->name,shadow_main_tag,pointer_tag,state->name);
    } else {
      expr(dfp,"for(k=0;k<%d;k++)",shadow_length);
      hang_expr(dfp,"%s_%s(%s,i,j,%s,k) = localsp[k];",gm->name,shadow_main_tag,pointer_tag,state->name);
    }

    add_block_comment(dfp,"Now figure out if any specials need this score");

    if( use_special == FALSE ) {
      add_block_comment(dfp,"Finished calculating state %s",state->name);
      continue; /** back to for i over state **/
    }

    
    /************************/
    /* for each special     */
    /* thats has this as    */
    /* source we have to    */
    /* update               */
    /************************/
    
    for(j=0;j<gm->spec_len;j++) {
      auto CellState * specstate;
      specstate = gm->special[j];
      
      
      for(k=0;k<specstate->len;k++) {
	if( strcmp(specstate->source[k]->state_source,state->name) == 0) {
	  /********************************/
	  /* is a special source!         */
	  /********************************/
	  add_break(dfp);
	  add_block_comment(dfp,"state %s is a source for special %s",state->name,specstate->name);
	  expr(dfp,"temp = score + (%s) + (%s) ",specstate->source[k]->calc_expr,specstate->calc_expr == NULL ? "0" : specstate->calc_expr );
	  expr(dfp,"if( temp > %s_%s(%s,i,j,%s) ) ",gm->name,specialtag,pointer_tag,specstate->name);
	  startbrace(dfp);
	  expr(dfp,"%s_%s(%s,i,j,%s) = temp",gm->name,specialtag,pointer_tag,specstate->name);
	  
	  if(shadow_on_special == TRUE ) {
	    add_block_comment(dfp,"Have to push only bottem half of system here");
	    expr(dfp,"for(k=0;k<3;k++)");
	      hang_expr(dfp,"%s_%s(%s,i,j,%s,k) = %s_%s(%s,i,j,%s,k);",
			gm->name,shadow_special_tag,pointer_tag,
			specstate->name, gm->name,
			specstate->source[k]->isspecial == TRUE ? shadow_special_tag : shadow_main_tag,
			pointer_tag,state->name);

	    /*** sort of hacky ***/
	    expr(dfp,"%s_%s(%s,i,j,%s,6) = %s_%s(%s,i,j,%s,6);",
			gm->name,shadow_special_tag,pointer_tag,
			specstate->name, gm->name,
			specstate->source[k]->isspecial == TRUE ? shadow_special_tag : shadow_main_tag,
			pointer_tag,state->name);


	    expr(dfp,"%s_%s(%s,i,j,%s,3) = i;",gm->name,shadow_special_tag,pointer_tag,specstate->name);
	    expr(dfp,"%s_%s(%s,i,j,%s,4) = j;",gm->name,shadow_special_tag,pointer_tag,specstate->name);
	    expr(dfp,"%s_%s(%s,i,j,%s,5) = %s;",gm->name,shadow_special_tag,pointer_tag,specstate->name,state->name);
	  }
	  else { /** not pushing on specials, pushing elsewhere **/

	    

	    expr(dfp,"for(k=0;k<%d;k++)",shadow_length);
	    hang_expr(dfp,"%s_%s(%s,i,j,%s,k) = %s_%s(%s,i,j,%s,k);",gm->name,shadow_special_tag,pointer_tag,
		    specstate->name, gm->name,specstate->source[k]->isspecial == TRUE ? shadow_special_tag : shadow_main_tag,
		      pointer_tag,state->name);
	  }

	  closebrace(dfp);
	  add_break(dfp);
	  
	}
      }
    }
    
    
    add_break(dfp);
    add_block_comment(dfp,"Finished calculating state %s",state->name);
    
  } /* end of for each state */
	
}



# line 2643 "dynashadow.dy"
void write_special_shadow_block(DYNFILE * dfp,GenericMatrix * gm,char * matrix,char * pointer_tag,char * special,char *  shadow_main_tag,char * shadow_special_tag,int shadow_length,boolean shadow_on_special,int debug)
{
  register int i;
  register int j;
  

  for(i=0;i<gm->spec_len;i++) {
    auto CellState * state;
    state = gm->special[i];
    add_break(dfp);
    if( state->specialtospecial == FALSE ) {
      add_block_comment(dfp,"Special state %s has no special to special movements",state->name);
      continue;
    }
    
    add_block_comment(dfp,"Special state %s has special to speical",state->name);
    
    add_block_comment(dfp,"Set score to current score (remember, state probably updated during main loop");
    expr(dfp,"score = %s_%s(mat,0,j,%s);",gm->name,special,state->name);
    add_break(dfp);
    for(j=0;j < state->len;j++) {
      auto CellSource * source;
      source = state->source[j];
      
      if( source->isspecial == FALSE ) {
	add_block_comment(dfp,"Source %s for state %s is not special... already calculated",source->state_source,state->name);
	continue; /** back to for j **/
      }

      add_block_comment(dfp,"Source %s is a special source for %s",source->state_source,state->name);
      expr(dfp,"temp = %s_%s(%s,0,j - %d,%s) + (%s) + (%s)",gm->name,special,pointer_tag,source->offj,source->state_source,
source->calc_expr,state->calc_expr == NULL ? "0" : state->calc_expr);
      expr(dfp,"if( temp > score )");
      startbrace(dfp);
      expr(dfp,"score = temp");
      add_block_comment(dfp,"Also got to propagate shadows ");
      expr(dfp,"for(k=0;k<%d;k++)",shadow_length);
      hang_expr(dfp,"%s_%s(%s,i,j,%s,k) = %s_%s(%s,i - %d,j - %d,%s,k);",
		gm->name,shadow_special_tag,pointer_tag,state->name,
		gm->name,shadow_special_tag,pointer_tag,state->source[j]->offi,
		state->source[j]->offj,state->source[j]->state_source);
      closebrace(dfp);

      
      add_break(dfp);
    }
    add_block_comment(dfp,"Put back score... (now updated!)");
    expr(dfp,"%s_%s(mat,0,j,%s) = score;",gm->name,special,state->name);
    add_block_comment(dfp,"Finished updating state %s",state->name);
    add_break(dfp);
  }
}


# line 2759 "dynashadow.c"

#ifdef _cplusplus
}
#endif
