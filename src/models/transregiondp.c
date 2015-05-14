#ifdef _cplusplus
extern "C" {
#endif
#include "transregiondp.h"


# line 81 "transregiondp.dy"
SequenceBaseCoverage * new_SequenceBaseCoverage(TransFactorMatchSet * tfms)
{
  int i;
  int j;
  int seqpos;
  SequenceBaseCoverage * out;


  out = SequenceBaseCoverage_alloc();
  out->coverage = calloc(tfms->target->len,sizeof(char));
  out->len = tfms->target->len;

  out->gc_points = calloc(tfms->target->len,sizeof(char));
  for(j=1;j<out->len;j++) {
    if( toupper(tfms->target->seq[j-1]) == 'G' && toupper(tfms->target->seq[j]) == 'C') {
      out->gc_points[j] = 1;
    }
  }

  for(i=0;i<tfms->len;i++) {
    for(j=tfms->match[i]->start;j<tfms->match[i]->end;j++) {
      if(out->coverage[j] < 100 ) {
	out->coverage[j]++;
      }
    }
  }



  return out;


}


# line 116 "transregiondp.dy"
TransRegionModel * new_logodds_TransRegionModel(double in_region_prob,double out_region_prob,double in_cost,double gc_region_ratio)
{
  TransRegionModel * out;


  out = TransRegionModel_alloc();

  out->region_present = Probability2Score(in_region_prob/out_region_prob);
  out->region_notpresent = Probability2Score((1.0-in_region_prob)/(1.0 - out_region_prob));

  out->nonregion_present =  Probability2Score(1.0);
  out->nonregion_notpresent = Probability2Score(1.0);

  out->region_start = Probability2Score(in_cost);

  out->gc_point = Probability2Score(gc_region_ratio);
  out->gc_non_point = 0;

  out->len = 1;
  fprintf(stderr,"... %d (%.2f) covered, %d (%.2f) not covered\n",out->region_present,Score2Bits(out->region_present),out->region_notpresent,Score2Bits(out->region_notpresent));

  return out;
}


# line 68 "transregiondp.c"
/* Function:  hard_link_SequenceBaseCoverage(obj)
 *
 * Descrip:    Bumps up the reference count of the object
 *             Meaning that multiple pointers can 'own' it
 *
 *
 * Arg:        obj [UNKN ] Object to be hard linked [SequenceBaseCoverage *]
 *
 * Return [UNKN ]  Undocumented return value [SequenceBaseCoverage *]
 *
 */
SequenceBaseCoverage * hard_link_SequenceBaseCoverage(SequenceBaseCoverage * obj) 
{
    if( obj == NULL )    {  
      warn("Trying to hard link to a SequenceBaseCoverage object: passed a NULL object");    
      return NULL;   
      }  
    obj->dynamite_hard_link++;   
    return obj;  
}    


/* Function:  SequenceBaseCoverage_alloc(void)
 *
 * Descrip:    Allocates structure: assigns defaults if given 
 *
 *
 *
 * Return [UNKN ]  Undocumented return value [SequenceBaseCoverage *]
 *
 */
SequenceBaseCoverage * SequenceBaseCoverage_alloc(void) 
{
    SequenceBaseCoverage * out; /* out is exported at end of function */ 


    /* call ckalloc and see if NULL */ 
    if((out=(SequenceBaseCoverage *) ckalloc (sizeof(SequenceBaseCoverage))) == NULL)    {  
      warn("SequenceBaseCoverage_alloc failed ");    
      return NULL;  /* calling function should respond! */ 
      }  
    out->dynamite_hard_link = 1; 
#ifdef PTHREAD   
    pthread_mutex_init(&(out->dynamite_mutex),NULL);     
#endif   
    out->coverage = NULL;    
    out->gc_points = NULL;   
    out->len = 0;    
    out->seq = NULL; 


    return out;  
}    


/* Function:  free_SequenceBaseCoverage(obj)
 *
 * Descrip:    Free Function: removes the memory held by obj
 *             Will chain up to owned members and clear all lists
 *
 *
 * Arg:        obj [UNKN ] Object that is free'd [SequenceBaseCoverage *]
 *
 * Return [UNKN ]  Undocumented return value [SequenceBaseCoverage *]
 *
 */
SequenceBaseCoverage * free_SequenceBaseCoverage(SequenceBaseCoverage * obj) 
{
    int return_early = 0;    


    if( obj == NULL) {  
      warn("Attempting to free a NULL pointer to a SequenceBaseCoverage obj. Should be trappable");  
      return NULL;   
      }  


#ifdef PTHREAD   
    assert(pthread_mutex_lock(&(obj->dynamite_mutex)) == 0); 
#endif   
    if( obj->dynamite_hard_link > 1)     {  
      return_early = 1;  
      obj->dynamite_hard_link--; 
      }  
#ifdef PTHREAD   
    assert(pthread_mutex_unlock(&(obj->dynamite_mutex)) == 0);   
#endif   
    if( return_early == 1)   
      return NULL;   
    if( obj->coverage != NULL)   
      ckfree(obj->coverage);     
    if( obj->gc_points != NULL)  
      ckfree(obj->gc_points);    
    if( obj->seq != NULL)    
      free_Sequence(obj->seq);   


    ckfree(obj); 
    return NULL; 
}    


/* Function:  hard_link_TransRegionModel(obj)
 *
 * Descrip:    Bumps up the reference count of the object
 *             Meaning that multiple pointers can 'own' it
 *
 *
 * Arg:        obj [UNKN ] Object to be hard linked [TransRegionModel *]
 *
 * Return [UNKN ]  Undocumented return value [TransRegionModel *]
 *
 */
TransRegionModel * hard_link_TransRegionModel(TransRegionModel * obj) 
{
    if( obj == NULL )    {  
      warn("Trying to hard link to a TransRegionModel object: passed a NULL object");    
      return NULL;   
      }  
    obj->dynamite_hard_link++;   
    return obj;  
}    


/* Function:  TransRegionModel_alloc(void)
 *
 * Descrip:    Allocates structure: assigns defaults if given 
 *
 *
 *
 * Return [UNKN ]  Undocumented return value [TransRegionModel *]
 *
 */
TransRegionModel * TransRegionModel_alloc(void) 
{
    TransRegionModel * out; /* out is exported at end of function */ 


    /* call ckalloc and see if NULL */ 
    if((out=(TransRegionModel *) ckalloc (sizeof(TransRegionModel))) == NULL)    {  
      warn("TransRegionModel_alloc failed ");    
      return NULL;  /* calling function should respond! */ 
      }  
    out->dynamite_hard_link = 1; 
#ifdef PTHREAD   
    pthread_mutex_init(&(out->dynamite_mutex),NULL);     
#endif   
    out->len = 0;    
    out->region_present = 0; 
    out->region_notpresent = 0;  
    out->nonregion_present = 0;  
    out->nonregion_notpresent = 0;   
    out->region_start = 0;   
    out->gc_point = 0;   
    out->gc_non_point = 0;   


    return out;  
}    


/* Function:  free_TransRegionModel(obj)
 *
 * Descrip:    Free Function: removes the memory held by obj
 *             Will chain up to owned members and clear all lists
 *
 *
 * Arg:        obj [UNKN ] Object that is free'd [TransRegionModel *]
 *
 * Return [UNKN ]  Undocumented return value [TransRegionModel *]
 *
 */
TransRegionModel * free_TransRegionModel(TransRegionModel * obj) 
{
    int return_early = 0;    


    if( obj == NULL) {  
      warn("Attempting to free a NULL pointer to a TransRegionModel obj. Should be trappable");  
      return NULL;   
      }  


#ifdef PTHREAD   
    assert(pthread_mutex_lock(&(obj->dynamite_mutex)) == 0); 
#endif   
    if( obj->dynamite_hard_link > 1)     {  
      return_early = 1;  
      obj->dynamite_hard_link--; 
      }  
#ifdef PTHREAD   
    assert(pthread_mutex_unlock(&(obj->dynamite_mutex)) == 0);   
#endif   
    if( return_early == 1)   
      return NULL;   


    ckfree(obj); 
    return NULL; 
}    




  /*****************   C functions  ****************/
  /*             Written using dynamite            */
  /*            Sat Sep  8 09:05:33 2007           */
  /*            email birney@sanger.ac.uk          */
  /* http://www.sanger.ac.uk/Users/birney/dynamite */
  /*************************************************/


  /* Please report any problems or bugs to         */
  /* Ewan Birney, birney@sanger.ac.uk              */


/* basic set of macros to map states to numbers */ 
#define REGION 0 
#define NON_REGION 1 


#define START 0  
#define END 1    


#define TransRegionMatrix_EXPL_MATRIX(this_matrix,i,j,STATE) this_matrix->basematrix->matrix[((j+1)*2)+STATE][i+0]   
#define TransRegionMatrix_EXPL_SPECIAL(matrix,i,j,STATE) matrix->basematrix->specmatrix[STATE][j+1]  
#define TransRegionMatrix_READ_OFF_ERROR -2
 


#define TransRegionMatrix_VSMALL_MATRIX(mat,i,j,STATE) mat->basematrix->matrix[(j+2)%2][((i+0)*2)+STATE] 
#define TransRegionMatrix_VSMALL_SPECIAL(mat,i,j,STATE) mat->basematrix->specmatrix[(j+2)%2][STATE]  




#define TransRegionMatrix_SHATTER_SPECIAL(matrix,i,j,STATE) matrix->shatter->special[STATE][j]   
#define TransRegionMatrix_SHATTER_MATRIX(matrix,i,j,STATE)  fetch_cell_value_ShatterMatrix(mat->shatter,i,j,STATE)   


/* Function:  PackAln_read_Shatter_TransRegionMatrix(mat)
 *
 * Descrip:    Reads off PackAln from shatter matrix structure
 *
 *
 * Arg:        mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 *
 * Return [UNKN ]  Undocumented return value [PackAln *]
 *
 */
PackAln * PackAln_read_Shatter_TransRegionMatrix(TransRegionMatrix * mat) 
{
    TransRegionMatrix_access_func_holder holder;     


    holder.access_main    = TransRegionMatrix_shatter_access_main;   
    holder.access_special = TransRegionMatrix_shatter_access_special;    
    assert(mat);     
    assert(mat->shatter);    
    return PackAln_read_generic_TransRegionMatrix(mat,holder);   
}    


/* Function:  TransRegionMatrix_shatter_access_main(mat,i,j,state)
 *
 * Descrip: No Description
 *
 * Arg:          mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:            i [UNKN ] Undocumented argument [int]
 * Arg:            j [UNKN ] Undocumented argument [int]
 * Arg:        state [UNKN ] Undocumented argument [int]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int TransRegionMatrix_shatter_access_main(TransRegionMatrix * mat,int i,int j,int state) 
{
    return TransRegionMatrix_SHATTER_MATRIX(mat,i,j,state);  
}    


/* Function:  TransRegionMatrix_shatter_access_special(mat,i,j,state)
 *
 * Descrip: No Description
 *
 * Arg:          mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:            i [UNKN ] Undocumented argument [int]
 * Arg:            j [UNKN ] Undocumented argument [int]
 * Arg:        state [UNKN ] Undocumented argument [int]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int TransRegionMatrix_shatter_access_special(TransRegionMatrix * mat,int i,int j,int state) 
{
    return TransRegionMatrix_SHATTER_SPECIAL(mat,i,j,state); 
}    


/* Function:  calculate_shatter_TransRegionMatrix(mat,dpenv)
 *
 * Descrip:    This function calculates the TransRegionMatrix matrix when in shatter mode
 *
 *
 * Arg:          mat [UNKN ] (null) [TransRegionMatrix *]
 * Arg:        dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
boolean calculate_shatter_TransRegionMatrix(TransRegionMatrix * mat,DPEnvelope * dpenv) 
{
    int i;   
    int j;   
    int k;   
    int should_calc;     
    int leni;    
    int lenj;    
    int tot; 
    int num; 
    int starti;  
    int startj;  
    int endi;    
    int endj;    


    int * SIG_0_0;   
    int * SIG_0_1;   


    leni = mat->leni;    
    lenj = mat->lenj;    


    mat->shatter = new_ShatterMatrix(dpenv,2,lenj,2);    
    prepare_DPEnvelope(dpenv);   
    starti = dpenv->starti;  
    if( starti < 0 ) 
      starti = 0;    
    startj = dpenv->startj;  
    if( startj < 0 ) 
      startj = 0;    
    endi = dpenv->endi;  
    if( endi > mat->leni )   
      endi = mat->leni;  
    endj = dpenv->endj;  
    if( endj > mat->lenj )   
      endj = mat->lenj;  
    tot = (endi-starti) * (endj-startj); 
    num = 0; 


    start_reporting("TransRegionMatrix Matrix calculation: ");   
    for(j=startj;j<endj;j++) {  
      auto int score;    
      auto int temp;     
      for(i=starti;i<endi;i++)   {  
        /* Check if is in envelope - code identical to is_in_DPEnvelope, but aggressively inlined here for speed */ 
        should_calc = 0; 
        for(k=0;k<dpenv->len;k++)    {  
          auto DPUnit * u;   
          u = dpenv->dpu[k]; 
          switch(u->type)    {  
            case DPENV_RECT :    
              if( i >= u->starti && j >= u->startj && i < (u->starti+u->height) && j < (u->startj+u->length))    
                should_calc = 1;     
              break; 
            case DPENV_DIAG :    
              if(  abs( (i-j) - (u->starti-u->startj)) <= u->height && i+j >= u->starti+u->startj && i+j+u->length >= u->starti+u->startj)   
                should_calc = 1;     
              break; 
            }  
          if( should_calc == 1 ) 
            break;   
          }  
        if( should_calc == 0)    
          continue;  


        SIG_0_0 = fetch_cell_from_ShatterMatrix(mat->shatter,i,j);   
        SIG_0_1 = fetch_cell_from_ShatterMatrix(mat->shatter,i-0,j-1);   




        /* For state REGION */ 
        /* setting first movement to score */ 
        score = SIG_0_1[REGION] + (TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model));    
        /* From state START to state REGION */ 
        temp = TransRegionMatrix_SHATTER_SPECIAL(mat,i-0,j-1,START) + (TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model));    
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state NON_REGION to state REGION */ 
        temp = SIG_0_1[NON_REGION] + mat->model->region_start;   
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for REGION */ 
        /* Add any movement independant score and put away */ 
         SIG_0_0[REGION] = score;    


        /* state REGION is a source for special END */ 
        temp = score + (0) + (0) ;   
        if( temp > TransRegionMatrix_SHATTER_SPECIAL(mat,i,j,END) )  {  
          TransRegionMatrix_SHATTER_SPECIAL(mat,i,j,END) = temp;     
          }  




        /* Finished calculating state REGION */ 


        /* For state NON_REGION */ 
        /* setting first movement to score */ 
        score = SIG_0_1[NON_REGION] + TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model);   
        /* From state START to state NON_REGION */ 
        temp = TransRegionMatrix_SHATTER_SPECIAL(mat,i-0,j-1,START) + TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model);   
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state REGION to state NON_REGION */ 
        temp = SIG_0_1[REGION] + 0;  
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for NON_REGION */ 
        /* Add any movement independant score and put away */ 
         SIG_0_0[NON_REGION] = score;    


        /* state NON_REGION is a source for special END */ 
        temp = score + (0) + (0) ;   
        if( temp > TransRegionMatrix_SHATTER_SPECIAL(mat,i,j,END) )  {  
          TransRegionMatrix_SHATTER_SPECIAL(mat,i,j,END) = temp;     
          }  




        /* Finished calculating state NON_REGION */ 
        }  


      /* Special state START has no special to special movements */ 


      /* Special state END has no special to special movements */ 
      }  
    stop_reporting();    
    return TRUE;     
}    


/* Function:  search_TransRegionMatrix(dbsi,out,model,seq)
 *
 * Descrip:    This function makes a database search of TransRegionMatrix
 *             It uses the dbsi structure to choose which implementation
 *             to use of the database searching. This way at run time you
 *             can switch between single threaded/multi-threaded or hardware
 *
 *
 * Arg:         dbsi [UNKN ] Undocumented argument [DBSearchImpl *]
 * Arg:          out [UNKN ] Undocumented argument [Hscore *]
 * Arg:        model [UNKN ] Undocumented argument [TransRegionModel*]
 * Arg:          seq [UNKN ] Undocumented argument [SequenceBaseCoverage*]
 *
 * Return [UNKN ]  Undocumented return value [Search_Return_Type]
 *
 */
Search_Return_Type search_TransRegionMatrix(DBSearchImpl * dbsi,Hscore * out,TransRegionModel* model,SequenceBaseCoverage* seq ) 
{
#ifdef PTHREAD   
    int i;   
    int thr_no;  
    pthread_attr_t pat;  
    struct thread_pool_holder_TransRegionMatrix * holder;    
#endif   
    if( out == NULL )    {  
      warn("Passed in a null Hscore object into search_TransRegionMatrix. Can't process results!");  
      return SEARCH_ERROR;   
      }  
    if( dbsi == NULL )   {  
      warn("Passed in a null DBSearchImpl object into search_TransRegionMatrix. Can't process results!");    
      return SEARCH_ERROR;   
      }  
    if( dbsi->trace_level > 5 )  
      warn("Asking for trace level of %d in database search for TransRegionMatrix, but it was compiled with a trace level of -2139062144. Not all trace statements can be shown",dbsi->trace_level); 
    switch(dbsi->type)   { /*switch on implementation*/ 
      case DBSearchImpl_Serial : 
        return serial_search_TransRegionMatrix(out,model,seq );  
      case DBSearchImpl_Pthreads :   
#ifdef PTHREAD   
        holder = (struct thread_pool_holder_TransRegionMatrix *) ckalloc(sizeof(struct thread_pool_holder_TransRegionMatrix));   
        if( holder == NULL )     {  
          warn("Unable to allocated thread pool datastructure...");  
          return SEARCH_ERROR;   
          }  
        holder->out = out;   
        holder->dbsi = dbsi; 
        holder->model = model;   
        holder->seq = seq;   
        if( pthread_mutex_init(&(holder->input_lock),NULL) != 0 )    
        fatal("Unable to iniated input mutex lock"); 
        if( pthread_mutex_init(&(holder->output_lock),NULL) != 0 )   
        fatal("Unable to iniated output mutex lock");    
        /* Let us rock! */ 
        thr_no = number_of_threads_DBSearchImpl(dbsi);   
        holder->pool = ckcalloc (thr_no,sizeof(pthread_t));  
        if( holder->pool == NULL )   {  
          warn("Unable to allocated thread pools");  
          return SEARCH_ERROR;   
          }  
        /* Build a thread attribute to make sure we get the most out of SMP boxes */ 
        pthread_attr_init(&pat);     
        /* Give thread libraries a hint that threads should be kernel threads */ 
#ifndef __sgi /* SGI can't set system scope ... */   
#ifdef  HAS_PTHREAD_SETSCOPE 
        pthread_attr_setscope(&pat, PTHREAD_SCOPE_SYSTEM);   
#endif /* set scope */   
#endif /* sgi */ 
        /* Give thread libraries a hint that there are num of threads to run */ 
#ifdef HAS_PTHREAD_SETCONCURRENCY    
        pthread_setconcurrency(thr_no+1);    
#endif /* set concurrency */ 
        for(i=0;i<thr_no;i++)    {  
          if( pthread_create(holder->pool+i,&pat,thread_loop_TransRegionMatrix,(void *)holder) ) 
            fatal("Unable to create a thread!"); 
          }  
        /* Now - wait for all the threads to exit */ 
        for(i=0;i<thr_no;i++)    {  
          if( pthread_join(holder->pool[i],NULL) != 0 )  
            fatal("Unable to join a thread!");   
          }  
        /* Deallocate the thread structures */ 
        ckfree(holder->pool);    
        ckfree(holder);  
        return SEARCH_OK;    
#else /* not compiled with threads */    
        warn("You did not specifiy the PTHREAD compile when compiled the C code for TransRegionMatrix"); 
#endif /* finished threads */    
      default :  
        warn("database search implementation %s was not provided in the compiled dynamite file from TransRegionMatrix",impl_string_DBSearchImpl(dbsi));  
        return SEARCH_ERROR; 
      } /* end of switch on implementation */ 


}    


/* Function:  thread_loop_TransRegionMatrix(ptr)
 *
 * Descrip:    dummy loop code foreach thread for TransRegionMatrix
 *
 *
 * Arg:        ptr [UNKN ] Undocumented argument [void *]
 *
 * Return [UNKN ]  Undocumented return value [void *]
 *
 */
void * thread_loop_TransRegionMatrix(void * ptr) 
{
    fatal("dummy thread loop function"); 
}    


/* Function:  serial_search_TransRegionMatrix(out,model,seq)
 *
 * Descrip:    This function makes a database search of TransRegionMatrix
 *             It is a single processor implementation
 *
 *
 * Arg:          out [UNKN ] Undocumented argument [Hscore *]
 * Arg:        model [UNKN ] Undocumented argument [TransRegionModel*]
 * Arg:          seq [UNKN ] Undocumented argument [SequenceBaseCoverage*]
 *
 * Return [UNKN ]  Undocumented return value [Search_Return_Type]
 *
 */
Search_Return_Type serial_search_TransRegionMatrix(Hscore * out,TransRegionModel* model,SequenceBaseCoverage* seq ) 
{
    int db_status;   
    int score;   
    int query_pos = 0;   
    int target_pos = 0;  
    DataScore * ds;  


    push_errormsg_stack("Before any actual search in db searching"); 


    target_pos = 0;  




    /* No maximum length - allocated on-the-fly */ 
    score = score_only_TransRegionMatrix(model, seq );   
    if( should_store_Hscore(out,score) == TRUE )     { /*if storing datascore*/ 
      ds = new_DataScore_from_storage(out);  
      if( ds == NULL )   {  
        warn("TransRegionMatrix search had a memory error in allocating a new_DataScore (?a leak somewhere - DataScore is a very small datastructure");  
        return SEARCH_ERROR; 
        }  
      /* Now: add query/target information to the entry */ 
      ds->score = score;     
      add_Hscore(out,ds);    
      } /* end of if storing datascore */ 
    pop_errormsg_stack();    
    push_errormsg_stack("DB searching: just finished [Query Pos: %d] [Target Pos: %d]",query_pos,target_pos);    


    pop_errormsg_stack();    
    return SEARCH_OK;    
}    


/* Function:  score_only_TransRegionMatrix(model,seq)
 *
 * Descrip:    This function just calculates the score for the matrix
 *             I am pretty sure we can do this better, but hey, for the moment...
 *             It calls /allocate_TransRegionMatrix_only
 *
 *
 * Arg:        model [UNKN ] query data structure [TransRegionModel*]
 * Arg:          seq [UNKN ] target data structure [SequenceBaseCoverage*]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int score_only_TransRegionMatrix(TransRegionModel* model,SequenceBaseCoverage* seq ) 
{
    int bestscore = NEGI;    
    int i;   
    int j;   
    int k;   
    TransRegionMatrix * mat;     


    mat = allocate_TransRegionMatrix_only(model, seq );  
    if( mat == NULL )    {  
      warn("Memory allocation error in the db search - unable to communicate to calling function. this spells DIASTER!");    
      return NEGI;   
      }  
    if((mat->basematrix = BaseMatrix_alloc_matrix_and_specials(2,(mat->leni + 0) * 2,2,2)) == NULL)  {  
      warn("Score only matrix for TransRegionMatrix cannot be allocated, (asking for 1  by %d  cells)",mat->leni*2); 
      mat = free_TransRegionMatrix(mat);     
      return 0;  
      }  
    mat->basematrix->type = BASEMATRIX_TYPE_VERYSMALL;   


    /* Now, initiate matrix */ 
    for(j=0;j<3;j++) {  
      for(i=(-0);i<mat->leni;i++)    {  
        for(k=0;k<2;k++) 
          TransRegionMatrix_VSMALL_MATRIX(mat,i,j,k) = NEGI; 
        }  
      TransRegionMatrix_VSMALL_SPECIAL(mat,i,j,START) = 0;   
      TransRegionMatrix_VSMALL_SPECIAL(mat,i,j,END) = NEGI;  
      }  


    /* Ok, lets do-o-o-o-o it */ 


    for(j=0;j<mat->lenj;j++) { /*for all target positions*/ 
      auto int score;    
      auto int temp;     
      for(i=0;i<mat->leni;i++)   { /*for all query positions*/ 


        /* For state REGION */ 
        /* setting first movement to score */ 
        score = TransRegionMatrix_VSMALL_MATRIX(mat,i-0,j-1,REGION) + (TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model));    
        /* From state START to state REGION */ 
        temp = TransRegionMatrix_VSMALL_SPECIAL(mat,i-0,j-1,START) + (TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model));     
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state NON_REGION to state REGION */ 
        temp = TransRegionMatrix_VSMALL_MATRIX(mat,i-0,j-1,NON_REGION) + mat->model->region_start;   
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for REGION */ 
        /* Add any movement independant score and put away */ 
         TransRegionMatrix_VSMALL_MATRIX(mat,i,j,REGION) = score;    


        /* state REGION is a source for special END */ 
        temp = score + (0) + (0) ;   
        if( temp > TransRegionMatrix_VSMALL_SPECIAL(mat,i,j,END) )   {  
          TransRegionMatrix_VSMALL_SPECIAL(mat,i,j,END) = temp;  
          }  




        /* Finished calculating state REGION */ 


        /* For state NON_REGION */ 
        /* setting first movement to score */ 
        score = TransRegionMatrix_VSMALL_MATRIX(mat,i-0,j-1,NON_REGION) + TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model);   
        /* From state START to state NON_REGION */ 
        temp = TransRegionMatrix_VSMALL_SPECIAL(mat,i-0,j-1,START) + TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model);    
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state REGION to state NON_REGION */ 
        temp = TransRegionMatrix_VSMALL_MATRIX(mat,i-0,j-1,REGION) + 0;  
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for NON_REGION */ 
        /* Add any movement independant score and put away */ 
         TransRegionMatrix_VSMALL_MATRIX(mat,i,j,NON_REGION) = score;    


        /* state NON_REGION is a source for special END */ 
        temp = score + (0) + (0) ;   
        if( temp > TransRegionMatrix_VSMALL_SPECIAL(mat,i,j,END) )   {  
          TransRegionMatrix_VSMALL_SPECIAL(mat,i,j,END) = temp;  
          }  




        /* Finished calculating state NON_REGION */ 
        } /* end of for all query positions */ 




      /* Special state START has no special to special movements */ 


      /* Special state END has no special to special movements */ 
      if( bestscore < TransRegionMatrix_VSMALL_SPECIAL(mat,0,j,END) )    
        bestscore = TransRegionMatrix_VSMALL_SPECIAL(mat,0,j,END);   
      } /* end of for all target positions */ 


    mat = free_TransRegionMatrix(mat);   
    return bestscore;    
}    


/* Function:  PackAln_bestmemory_TransRegionMatrix(model,seq,dpenv,dpri)
 *
 * Descrip:    This function chooses the best memory set-up for the alignment
 *             using calls to basematrix, and then implements either a large
 *             or small memory model.
 *
 *             It is the best function to use if you just want an alignment
 *
 *             If you want a label alignment, you will need
 *             /convert_PackAln_to_AlnBlock_TransRegionMatrix
 *
 *
 * Arg:        model [UNKN ] query data structure [TransRegionModel*]
 * Arg:          seq [UNKN ] target data structure [SequenceBaseCoverage*]
 * Arg:        dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 * Arg:         dpri [UNKN ] Undocumented argument [DPRunImpl *]
 *
 * Return [UNKN ]  Undocumented return value [PackAln *]
 *
 */
PackAln * PackAln_bestmemory_TransRegionMatrix(TransRegionModel* model,SequenceBaseCoverage* seq ,DPEnvelope * dpenv,DPRunImpl * dpri) 
{
    long long total; 
    TransRegionMatrix * mat; 
    PackAln * out;   
    DebugMatrix * de;    
    DPRunImplMemory strategy;    
    assert(dpri);    


    total = model->len * seq->len;   
    if( dpri->memory == DPIM_Default )   {  
      if( (total * 2 * sizeof(int)) > 1000*dpri->kbyte_size) {  
        strategy = DPIM_Linear;  
        }  
      else   {  
        strategy = DPIM_Explicit;    
        }  
      }  
    else {  
      strategy = dpri->memory;   
      }  


    if( dpenv != NULL )  {  
      if( strategy == DPIM_Explicit) {  
        if( (mat=allocate_Expl_TransRegionMatrix(model, seq ,dpri)) == NULL )    {  
          warn("Unable to allocate large TransRegionMatrix version");    
          return NULL;   
          }  
        calculate_dpenv_TransRegionMatrix(mat,dpenv);    
        out =  PackAln_read_Expl_TransRegionMatrix(mat); 
        }  
      else   {  
        mat = allocate_TransRegionMatrix_only(model, seq );  
        calculate_shatter_TransRegionMatrix(mat,dpenv);  
        out = PackAln_read_Shatter_TransRegionMatrix(mat);   
        }  
      }  
    else {  
      if( strategy == DPIM_Linear )  {  
        /* use small implementation */ 
        if( (mat=allocate_Small_TransRegionMatrix(model, seq )) == NULL )    {  
          warn("Unable to allocate small TransRegionMatrix version");    
          return NULL;   
          }  
        out = PackAln_calculate_Small_TransRegionMatrix(mat,dpenv);  
        }  
      else   {  
        /* use Large implementation */ 
        if( (mat=allocate_Expl_TransRegionMatrix(model, seq ,dpri)) == NULL )    {  
          warn("Unable to allocate large TransRegionMatrix version");    
          return NULL;   
          }  
        if( dpri->debug == TRUE) {  
          fatal("Asked for dydebug, but dynamite file not compiled with -g. Need to recompile dynamite source"); 
          }  
        else {  
          calculate_TransRegionMatrix(mat);  
          out =  PackAln_read_Expl_TransRegionMatrix(mat);   
          }  
        }  
      }  


    mat = free_TransRegionMatrix(mat);   
    return out;  
}    


/* Function:  allocate_TransRegionMatrix_only(model,seq)
 *
 * Descrip:    This function only allocates the TransRegionMatrix structure
 *             checks types where possible and determines leni and lenj
 *             The basematrix area is delt with elsewhere
 *
 *
 * Arg:        model [UNKN ] query data structure [TransRegionModel*]
 * Arg:          seq [UNKN ] target data structure [SequenceBaseCoverage*]
 *
 * Return [UNKN ]  Undocumented return value [TransRegionMatrix *]
 *
 */
TransRegionMatrix * allocate_TransRegionMatrix_only(TransRegionModel* model,SequenceBaseCoverage* seq ) 
{
    TransRegionMatrix * out;     


    if((out= TransRegionMatrix_alloc()) == NULL) {  
      warn("Allocation of basic TransRegionMatrix structure failed..."); 
      return NULL;   
      }  


    out->model = model;  
    out->seq = seq;  
    out->leni = model->len;  
    out->lenj = seq->len;    
    return out;  
}    


/* Function:  allocate_Expl_TransRegionMatrix(model,seq,dpri)
 *
 * Descrip:    This function allocates the TransRegionMatrix structure
 *             and the basematrix area for explicit memory implementations
 *             It calls /allocate_TransRegionMatrix_only
 *
 *
 * Arg:        model [UNKN ] query data structure [TransRegionModel*]
 * Arg:          seq [UNKN ] target data structure [SequenceBaseCoverage*]
 * Arg:         dpri [UNKN ] Undocumented argument [DPRunImpl *]
 *
 * Return [UNKN ]  Undocumented return value [TransRegionMatrix *]
 *
 */
TransRegionMatrix * allocate_Expl_TransRegionMatrix(TransRegionModel* model,SequenceBaseCoverage* seq ,DPRunImpl * dpri) 
{
    TransRegionMatrix * out; 


    out = allocate_TransRegionMatrix_only(model, seq );  
    if( out == NULL )    
      return NULL;   
    if( dpri->should_cache == TRUE ) {  
      if( dpri->cache != NULL )  {  
        if( dpri->cache->maxleni >= (out->lenj+1)*2 && dpri->cache->maxlenj >= (out->leni+0))    
          out->basematrix = hard_link_BaseMatrix(dpri->cache);   
        else 
          dpri->cache = free_BaseMatrix(dpri->cache);    
        }  
      }  
    if( out->basematrix == NULL )    {  
      if( (out->basematrix = BaseMatrix_alloc_matrix_and_specials((out->lenj+1)*2,(out->leni+0),2,out->lenj+1)) == NULL) {  
        warn("Explicit matrix TransRegionMatrix cannot be allocated, (asking for %d by %d main cells)",out->leni,out->lenj); 
        free_TransRegionMatrix(out);     
        return NULL; 
        }  
      }  
    if( dpri->should_cache == TRUE && dpri->cache == NULL)   
      dpri->cache = hard_link_BaseMatrix(out->basematrix);   
    out->basematrix->type = BASEMATRIX_TYPE_EXPLICIT;    
    init_TransRegionMatrix(out);     
    return out;  
}    


/* Function:  init_TransRegionMatrix(mat)
 *
 * Descrip:    This function initates TransRegionMatrix matrix when in explicit mode
 *             Called in /allocate_Expl_TransRegionMatrix
 *
 *
 * Arg:        mat [UNKN ] TransRegionMatrix which contains explicit basematrix memory [TransRegionMatrix *]
 *
 */
void init_TransRegionMatrix(TransRegionMatrix * mat) 
{
    register int i;  
    register int j;  
    if( mat->basematrix->type != BASEMATRIX_TYPE_EXPLICIT)   {  
      warn("Cannot iniate matrix, is not an explicit memory type and you have assummed that");   
      return;    
      }  


    for(i= (-0);i<mat->model->len;i++)   {  
      for(j= (-1);j<2;j++)   {  
        TransRegionMatrix_EXPL_MATRIX(mat,i,j,REGION) = NEGI;    
        TransRegionMatrix_EXPL_MATRIX(mat,i,j,NON_REGION) = NEGI;    
        }  
      }  
    for(j= (-1);j<mat->seq->len;j++) {  
      for(i= (-0);i<1;i++)   {  
        TransRegionMatrix_EXPL_MATRIX(mat,i,j,REGION) = NEGI;    
        TransRegionMatrix_EXPL_MATRIX(mat,i,j,NON_REGION) = NEGI;    
        }  
      TransRegionMatrix_EXPL_SPECIAL(mat,i,j,START) = 0; 
      TransRegionMatrix_EXPL_SPECIAL(mat,i,j,END) = NEGI;    
      }  
    return;  
}    


/* Function:  recalculate_PackAln_TransRegionMatrix(pal,mat)
 *
 * Descrip:    This function recalculates the PackAln structure produced by TransRegionMatrix
 *             For example, in linear space methods this is used to score them
 *
 *
 * Arg:        pal [UNKN ] Undocumented argument [PackAln *]
 * Arg:        mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 *
 */
void recalculate_PackAln_TransRegionMatrix(PackAln * pal,TransRegionMatrix * mat) 
{
    int i,j,k,offi,offj; 
    PackAlnUnit * prev;  
    PackAlnUnit * pau;   


    for(k=1,prev=pal->pau[0];k < pal->len;k++,prev=pau)  {  
      pau = pal->pau[k]; 
      i = pau->i;    
      j = pau->j;    
      offi = pau->i - prev->i;   
      offj = pau->j - prev->j;   
      switch(pau->state) {  
        case REGION :    
          if( offi == 0 && offj == 1 && prev->state == REGION )  {  
            pau->score = (TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model)) + (0);   
            continue;    
            }  
          if( offj == 1 && prev->state == (START+2) )    {  
            pau->score = (TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model)) + (0);   
            continue;    
            }  
          if( offi == 0 && offj == 1 && prev->state == NON_REGION )  {  
            pau->score = mat->model->region_start + (0);     
            continue;    
            }  
          warn("In recaluclating PackAln with state REGION, from [%d,%d,%d], got a bad source state. Error!",offi,offj,prev->state); 
          break; 
        case NON_REGION :    
          if( offi == 0 && offj == 1 && prev->state == NON_REGION )  {  
            pau->score = TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model) + (0);  
            continue;    
            }  
          if( offj == 1 && prev->state == (START+2) )    {  
            pau->score = TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model) + (0);  
            continue;    
            }  
          if( offi == 0 && offj == 1 && prev->state == REGION )  {  
            pau->score = 0 + (0);    
            continue;    
            }  
          warn("In recaluclating PackAln with state NON_REGION, from [%d,%d,%d], got a bad source state. Error!",offi,offj,prev->state); 
          break; 
        case (START+2) :     
          warn("In recaluclating PackAln with state START, got a bad source state. Error!"); 
          break; 
        case (END+2) :   
          if( offj == 0 && prev->state == NON_REGION )   {  
            /* i here comes from the previous state ;) - not the real one */ 
            i = prev->i; 
            pau->score = 0 + (0);    
            continue;    
            }  
          if( offj == 0 && prev->state == REGION )   {  
            /* i here comes from the previous state ;) - not the real one */ 
            i = prev->i; 
            pau->score = 0 + (0);    
            continue;    
            }  
          warn("In recaluclating PackAln with state END, got a bad source state. Error!");   
          break; 
        default :    
          warn("In recaluclating PackAln got a bad recipient state. Error!");    
        }  
      prev = pau;    
      }  
    return;  
}    
/* divide and conquor macros are next */ 
#define TransRegionMatrix_HIDDEN_MATRIX(thismatrix,i,j,state) (thismatrix->basematrix->matrix[(j-hiddenj+1)][(i+0)*2+state]) 
#define TransRegionMatrix_DC_SHADOW_MATRIX(thismatrix,i,j,state) (thismatrix->basematrix->matrix[((j+2)*8) % 16][(i+0)*2+state]) 
#define TransRegionMatrix_HIDDEN_SPECIAL(thismatrix,i,j,state) (thismatrix->basematrix->specmatrix[state][(j+1)])    
#define TransRegionMatrix_DC_SHADOW_SPECIAL(thismatrix,i,j,state) (thismatrix->basematrix->specmatrix[state*8][(j+1)])   
#define TransRegionMatrix_DC_SHADOW_MATRIX_SP(thismatrix,i,j,state,shadow) (thismatrix->basematrix->matrix[((((j+2)*8)+(shadow+1)) % 16)][(i+0)*2 + state])  
#define TransRegionMatrix_DC_SHADOW_SPECIAL_SP(thismatrix,i,j,state,shadow) (thismatrix->basematrix->specmatrix[state*8 +shadow+1][(j+1)])   
#define TransRegionMatrix_DC_OPT_SHADOW_MATRIX(thismatrix,i,j,state) (score_pointers[(((j+1)% 1) * (leni+1) * 2) + ((i+0) * 2) + (state)])   
#define TransRegionMatrix_DC_OPT_SHADOW_MATRIX_SP(thismatrix,i,j,state,shadow) (shadow_pointers[(((j+1)% 1) * (leni+1) * 16) + ((i+0) * 16) + (state * 8) + shadow+1])   
#define TransRegionMatrix_DC_OPT_SHADOW_SPECIAL(thismatrix,i,j,state) (thismatrix->basematrix->specmatrix[state*8][(j+1)])   
/* Function:  allocate_Small_TransRegionMatrix(model,seq)
 *
 * Descrip:    This function allocates the TransRegionMatrix structure
 *             and the basematrix area for a small memory implementations
 *             It calls /allocate_TransRegionMatrix_only
 *
 *
 * Arg:        model [UNKN ] query data structure [TransRegionModel*]
 * Arg:          seq [UNKN ] target data structure [SequenceBaseCoverage*]
 *
 * Return [UNKN ]  Undocumented return value [TransRegionMatrix *]
 *
 */
#define TransRegionMatrix_DC_OPT_SHADOW_SPECIAL_SP(thismatrix,i,j,state,shadow) (thismatrix->basematrix->specmatrix[state*8 +shadow+1][(j+1)])   
TransRegionMatrix * allocate_Small_TransRegionMatrix(TransRegionModel* model,SequenceBaseCoverage* seq ) 
{
    TransRegionMatrix * out; 


    out = allocate_TransRegionMatrix_only(model, seq );  
    if( out == NULL )    
      return NULL;   
    out->basematrix = BaseMatrix_alloc_matrix_and_specials(16,(out->leni + 0) * 2,16,out->lenj+1);   
    if(out == NULL)  {  
      warn("Small shadow matrix TransRegionMatrix cannot be allocated, (asking for 2 by %d main cells)",out->leni+1);    
      free_TransRegionMatrix(out);   
      return NULL;   
      }  
    out->basematrix->type = BASEMATRIX_TYPE_SHADOW;  
    return out;  
}    


/* Function:  PackAln_calculate_Small_TransRegionMatrix(mat,dpenv)
 *
 * Descrip:    This function calculates an alignment for TransRegionMatrix structure in linear space
 *             If you want only the start/end points
 *             use /AlnRangeSet_calculate_Small_TransRegionMatrix 
 *
 *             The function basically
 *               finds start/end points 
 *               foreach start/end point 
 *                 calls /full_dc_TransRegionMatrix 
 *
 *
 * Arg:          mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:        dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 *
 * Return [UNKN ]  Undocumented return value [PackAln *]
 *
 */
PackAln * PackAln_calculate_Small_TransRegionMatrix(TransRegionMatrix * mat,DPEnvelope * dpenv) 
{
    int endj;    
    int score;   
    PackAln * out;   
    PackAlnUnit * pau;   
    int starti;  
    int startj;  
    int startstate;  
    int stopi;   
    int stopj;   
    int stopstate;   
    int temp;    
    int donej;  /* This is for reporting, will be passed as a & arg in */ 
    int totalj; /* This also is for reporting, but as is not changed, can be passed by value */ 


    if( mat->basematrix->type != BASEMATRIX_TYPE_SHADOW )    {  
      warn("Could not calculate packaln small for TransRegionMatrix due to wrong type of matrix");   
      return NULL;   
      }  


    out = PackAln_alloc_std();   


    start_reporting("Find start end points: ");  
    dc_optimised_start_end_calc_TransRegionMatrix(mat,dpenv);    
    score = start_end_find_end_TransRegionMatrix(mat,&endj); 
    out->score = score;  
    stopstate = END;
    
    /* No special to specials: one matrix alignment: simply remove and get */ 
    starti = TransRegionMatrix_DC_SHADOW_SPECIAL_SP(mat,0,endj,END,0);   
    startj = TransRegionMatrix_DC_SHADOW_SPECIAL_SP(mat,0,endj,END,1);   
    startstate = TransRegionMatrix_DC_SHADOW_SPECIAL_SP(mat,0,endj,END,2);   
    stopi = TransRegionMatrix_DC_SHADOW_SPECIAL_SP(mat,0,endj,END,3);    
    stopj = TransRegionMatrix_DC_SHADOW_SPECIAL_SP(mat,0,endj,END,4);    
    stopstate = TransRegionMatrix_DC_SHADOW_SPECIAL_SP(mat,0,endj,END,5);    
    temp = TransRegionMatrix_DC_SHADOW_SPECIAL_SP(mat,0,endj,END,6); 
    log_full_error(REPORT,0,"[%d,%d][%d,%d] Score %d",starti,startj,stopi,stopj,score);  
    stop_reporting();    
    start_reporting("Recovering alignment: ");   


    /* Figuring how much j we have to align for reporting purposes */ 
    donej = 0;   
    totalj = stopj - startj; 
    full_dc_TransRegionMatrix(mat,starti,startj,startstate,stopi,stopj,stopstate,out,&donej,totalj,dpenv);   


    /* Although we have no specials, need to get start. Better to check than assume */ 


    max_matrix_to_special_TransRegionMatrix(mat,starti,startj,startstate,temp,&stopi,&stopj,&stopstate,&temp,NULL);  
    if( stopi == TransRegionMatrix_READ_OFF_ERROR || stopstate != START )    {  
      warn("Problem in reading off special state system, hit a non start state (or an internal error) in a single alignment mode");  
      invert_PackAln(out);   
      recalculate_PackAln_TransRegionMatrix(out,mat);    
      return out;    
      }  


    /* Ok. Put away start start... */ 
    pau = PackAlnUnit_alloc();   
    pau->i = stopi;  
    pau->j = stopj;  
    pau->state = stopstate + 2;  
    add_PackAln(out,pau);    


    log_full_error(REPORT,0,"Alignment recovered");  
    stop_reporting();    
    invert_PackAln(out); 
    recalculate_PackAln_TransRegionMatrix(out,mat);  
    return out;  


}    


/* Function:  AlnRangeSet_calculate_Small_TransRegionMatrix(mat)
 *
 * Descrip:    This function calculates an alignment for TransRegionMatrix structure in linear space
 *             If you want the full alignment, use /PackAln_calculate_Small_TransRegionMatrix 
 *             If you have already got the full alignment, but want the range set, use /AlnRangeSet_from_PackAln_TransRegionMatrix
 *             If you have got the small matrix but not the alignment, use /AlnRangeSet_from_TransRegionMatrix 
 *
 *
 * Arg:        mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 *
 * Return [UNKN ]  Undocumented return value [AlnRangeSet *]
 *
 */
AlnRangeSet * AlnRangeSet_calculate_Small_TransRegionMatrix(TransRegionMatrix * mat) 
{
    AlnRangeSet * out;   


    start_reporting("Find start end points: ");  
    dc_optimised_start_end_calc_TransRegionMatrix(mat,NULL); 
    log_full_error(REPORT,0,"Calculated");   


    out = AlnRangeSet_from_TransRegionMatrix(mat);   
    return out;  
}    


/* Function:  AlnRangeSet_from_TransRegionMatrix(mat)
 *
 * Descrip:    This function reads off a start/end structure
 *             for TransRegionMatrix structure in linear space
 *             If you want the full alignment use
 *             /PackAln_calculate_Small_TransRegionMatrix 
 *             If you have not calculated the matrix use
 *             /AlnRange_calculate_Small_TransRegionMatrix
 *
 *
 * Arg:        mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 *
 * Return [UNKN ]  Undocumented return value [AlnRangeSet *]
 *
 */
AlnRangeSet * AlnRangeSet_from_TransRegionMatrix(TransRegionMatrix * mat) 
{
    AlnRangeSet * out;   
    AlnRange * temp; 
    int jpos;    
    int state;   


    if( mat->basematrix->type != BASEMATRIX_TYPE_SHADOW) {  
      warn("Bad error! - non shadow matrix type in AlnRangeSet_from_TransRegionMatrix"); 
      return NULL;   
      }  


    out = AlnRangeSet_alloc_std();   
    /* Find the end position */ 
    out->score = start_end_find_end_TransRegionMatrix(mat,&jpos);    
    state = END; 


    while( (temp = AlnRange_build_TransRegionMatrix(mat,jpos,state,&jpos,&state)) != NULL)   
      add_AlnRangeSet(out,temp); 
    return out;  
}    


/* Function:  AlnRange_build_TransRegionMatrix(mat,stopj,stopspecstate,startj,startspecstate)
 *
 * Descrip:    This function calculates a single start/end set in linear space
 *             Really a sub-routine for /AlnRangeSet_from_PackAln_TransRegionMatrix
 *
 *
 * Arg:                   mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:                 stopj [UNKN ] Undocumented argument [int]
 * Arg:         stopspecstate [UNKN ] Undocumented argument [int]
 * Arg:                startj [UNKN ] Undocumented argument [int *]
 * Arg:        startspecstate [UNKN ] Undocumented argument [int *]
 *
 * Return [UNKN ]  Undocumented return value [AlnRange *]
 *
 */
AlnRange * AlnRange_build_TransRegionMatrix(TransRegionMatrix * mat,int stopj,int stopspecstate,int * startj,int * startspecstate) 
{
    AlnRange * out;  
    int jpos;    
    int state;   


    if( mat->basematrix->type != BASEMATRIX_TYPE_SHADOW) {  
      warn("Bad error! - non shadow matrix type in AlnRangeSet_from_TransRegionMatrix"); 
      return NULL;   
      }  


    /* Assumme that we have specials (we should!). Read back along the specials till we have the finish point */ 
    if( read_special_strip_TransRegionMatrix(mat,0,stopj,stopspecstate,&jpos,&state,NULL) == FALSE)  {  
      warn("In AlnRanger_build_TransRegionMatrix alignment ending at %d, unable to read back specials. Will (evenutally) return a partial range set... BEWARE!",stopj);  
      return NULL;   
      }  
    if( state == START || jpos <= 0) 
      return NULL;   


    out = AlnRange_alloc();  


    out->starti = TransRegionMatrix_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,0);    
    out->startj = TransRegionMatrix_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,1);    
    out->startstate = TransRegionMatrix_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,2);    
    out->stopi = TransRegionMatrix_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,3); 
    out->stopj = TransRegionMatrix_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,4); 
    out->stopstate = TransRegionMatrix_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,5); 
    out->startscore = TransRegionMatrix_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,6);    
    out->stopscore = TransRegionMatrix_DC_SHADOW_SPECIAL(mat,0,jpos,state);  


    /* Now, we have to figure out where this state came from in the specials */ 
    max_matrix_to_special_TransRegionMatrix(mat,out->starti,out->startj,out->startstate,out->startscore,&jpos,startj,startspecstate,&state,NULL);    
    if( jpos == TransRegionMatrix_READ_OFF_ERROR)    {  
      warn("In AlnRange_build_TransRegionMatrix alignment ending at %d, with aln range between %d-%d in j, unable to find source special, returning this range, but this could get tricky!",stopj,out->startj,out->stopj);   
      return out;    
      }  


    /* Put in the correct score for startstate, from the special */ 
    out->startscore = TransRegionMatrix_DC_SHADOW_SPECIAL(mat,0,*startj,*startspecstate);    
    /* The correct j coords have been put into startj, startspecstate... so just return out */ 
    return out;  
}    


/* Function:  read_hidden_TransRegionMatrix(mat,starti,startj,startstate,stopi,stopj,stopstate,out)
 *
 * Descrip: No Description
 *
 * Arg:               mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:            starti [UNKN ] Undocumented argument [int]
 * Arg:            startj [UNKN ] Undocumented argument [int]
 * Arg:        startstate [UNKN ] Undocumented argument [int]
 * Arg:             stopi [UNKN ] Undocumented argument [int]
 * Arg:             stopj [UNKN ] Undocumented argument [int]
 * Arg:         stopstate [UNKN ] Undocumented argument [int]
 * Arg:               out [UNKN ] Undocumented argument [PackAln *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
boolean read_hidden_TransRegionMatrix(TransRegionMatrix * mat,int starti,int startj,int startstate,int stopi,int stopj,int stopstate,PackAln * out) 
{
    int i;   
    int j;   
    int state;   
    int cellscore;   
    int isspecial;   
    /* We don't need hiddenj here, 'cause matrix access handled by max funcs */ 
    PackAlnUnit * pau;   


    /* stop position is on the path */ 
    i = stopi;   
    j = stopj;   
    state= stopstate;    
    isspecial = FALSE;   


    while( i >= starti && j >= startj)   {  
      /* Put away current i,j,state */ 
      pau = PackAlnUnit_alloc();/* Should deal with memory overflow */ 
      pau->i = i;    
      pau->j = j;    
      pau->state =  state;   
      add_PackAln(out,pau);  


      max_hidden_TransRegionMatrix(mat,startj,i,j,state,isspecial,&i,&j,&state,&isspecial,&cellscore);   


      if( i == TransRegionMatrix_READ_OFF_ERROR) {  
        warn("In TransRegionMatrix hidden read off, between %d:%d,%d:%d - at got bad read off. Problem!",starti,startj,stopi,stopj); 
        return FALSE;    
        }  


      if( i == starti && j == startj && state == startstate) {  
/* Put away final state (start of this block) */ 
        pau = PackAlnUnit_alloc();  /* Should deal with memory overflow */ 
        pau->i = i;  
        pau->j = j;  
        pau->state =  state; 
        add_PackAln(out,pau);    
          return TRUE;   
        }  
      if( i == starti && j == startj)    {  
        warn("In TransRegionMatrix hidden read off, between %d:%d,%d:%d - hit start cell, but not in start state. Can't be good!.",starti,startj,stopi,stopj);   
        return FALSE;    
        }  
      }  
    warn("In TransRegionMatrix hidden read off, between %d:%d,%d:%d - gone past start cell (now in %d,%d,%d), can't be good news!.",starti,startj,stopi,stopj,i,j,state);    
    return FALSE;    
}    


/* Function:  max_hidden_TransRegionMatrix(mat,hiddenj,i,j,state,isspecial,reti,retj,retstate,retspecial,cellscore)
 *
 * Descrip: No Description
 *
 * Arg:               mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:           hiddenj [UNKN ] Undocumented argument [int]
 * Arg:                 i [UNKN ] Undocumented argument [int]
 * Arg:                 j [UNKN ] Undocumented argument [int]
 * Arg:             state [UNKN ] Undocumented argument [int]
 * Arg:         isspecial [UNKN ] Undocumented argument [boolean]
 * Arg:              reti [UNKN ] Undocumented argument [int *]
 * Arg:              retj [UNKN ] Undocumented argument [int *]
 * Arg:          retstate [UNKN ] Undocumented argument [int *]
 * Arg:        retspecial [UNKN ] Undocumented argument [boolean *]
 * Arg:         cellscore [UNKN ] Undocumented argument [int *]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int max_hidden_TransRegionMatrix(TransRegionMatrix * mat,int hiddenj,int i,int j,int state,boolean isspecial,int * reti,int * retj,int * retstate,boolean * retspecial,int * cellscore) 
{
    register int temp;   
    register int cscore; 


    *reti = (*retj) = (*retstate) = TransRegionMatrix_READ_OFF_ERROR;    


    if( i < 0 || j < 0 || i > mat->model->len || j > mat->seq->len)  {  
      warn("In TransRegionMatrix matrix special read off - out of bounds on matrix [i,j is %d,%d state %d in standard matrix]",i,j,state);   
      return -1; 
      }  


    /* Then you have to select the correct switch statement to figure out the readoff      */ 
    /* Somewhat odd - reverse the order of calculation and return as soon as it is correct */ 
    cscore = TransRegionMatrix_HIDDEN_MATRIX(mat,i,j,state); 
    switch(state)    { /*Switch state */ 
      case REGION :  
        temp = cscore - (mat->model->region_start) -  (0);   
        if( temp == TransRegionMatrix_HIDDEN_MATRIX(mat,i - 0,j - 1,NON_REGION) )    {  
          *reti = i - 0; 
          *retj = j - 1; 
          *retstate = NON_REGION;    
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - TransRegionMatrix_HIDDEN_MATRIX(mat,i-0,j-1,NON_REGION);   
            }  
          return TransRegionMatrix_HIDDEN_MATRIX(mat,i - 0,j - 1,NON_REGION);    
          }  
        /* Not allowing special sources.. skipping START */ 
        temp = cscore - ((TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model))) -  (0); 
        if( temp == TransRegionMatrix_HIDDEN_MATRIX(mat,i - 0,j - 1,REGION) )    {  
          *reti = i - 0; 
          *retj = j - 1; 
          *retstate = REGION;    
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - TransRegionMatrix_HIDDEN_MATRIX(mat,i-0,j-1,REGION);   
            }  
          return TransRegionMatrix_HIDDEN_MATRIX(mat,i - 0,j - 1,REGION);    
          }  
        warn("Major problem (!) - in TransRegionMatrix read off, position %d,%d state %d no source found!",i,j,state);   
        return (-1); 
      case NON_REGION :  
        temp = cscore - (0) -  (0);  
        if( temp == TransRegionMatrix_HIDDEN_MATRIX(mat,i - 0,j - 1,REGION) )    {  
          *reti = i - 0; 
          *retj = j - 1; 
          *retstate = REGION;    
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - TransRegionMatrix_HIDDEN_MATRIX(mat,i-0,j-1,REGION);   
            }  
          return TransRegionMatrix_HIDDEN_MATRIX(mat,i - 0,j - 1,REGION);    
          }  
        /* Not allowing special sources.. skipping START */ 
        temp = cscore - (TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model)) -  (0);    
        if( temp == TransRegionMatrix_HIDDEN_MATRIX(mat,i - 0,j - 1,NON_REGION) )    {  
          *reti = i - 0; 
          *retj = j - 1; 
          *retstate = NON_REGION;    
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - TransRegionMatrix_HIDDEN_MATRIX(mat,i-0,j-1,NON_REGION);   
            }  
          return TransRegionMatrix_HIDDEN_MATRIX(mat,i - 0,j - 1,NON_REGION);    
          }  
        warn("Major problem (!) - in TransRegionMatrix read off, position %d,%d state %d no source found!",i,j,state);   
        return (-1); 
      default:   
        warn("Major problem (!) - in TransRegionMatrix read off, position %d,%d state %d no source found!",i,j,state);   
        return (-1); 
      } /* end of Switch state  */ 
}    


/* Function:  read_special_strip_TransRegionMatrix(mat,stopi,stopj,stopstate,startj,startstate,out)
 *
 * Descrip: No Description
 *
 * Arg:               mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:             stopi [UNKN ] Undocumented argument [int]
 * Arg:             stopj [UNKN ] Undocumented argument [int]
 * Arg:         stopstate [UNKN ] Undocumented argument [int]
 * Arg:            startj [UNKN ] Undocumented argument [int *]
 * Arg:        startstate [UNKN ] Undocumented argument [int *]
 * Arg:               out [UNKN ] Undocumented argument [PackAln *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
boolean read_special_strip_TransRegionMatrix(TransRegionMatrix * mat,int stopi,int stopj,int stopstate,int * startj,int * startstate,PackAln * out) 
{
    int i;   
    int j;   
    int state;   
    int cellscore;   
    int isspecial;   
    PackAlnUnit * pau;   


    /* stop position is on the path */ 
    i = stopi;   
    j = stopj;   
    state= stopstate;    
    isspecial = TRUE;    


    /* Loop until state has the same j as its stop in shadow pointers */ 
    /* This will be the state is came out from, OR it has hit !start */ 
    /* We may not want to get the alignment, in which case out will be NULL */ 
    while( j > TransRegionMatrix_DC_SHADOW_SPECIAL_SP(mat,i,j,state,4) && state != START)    { /*while more specials to eat up*/ 
      /* Put away current state, if we should */ 
      if(out != NULL)    {  
        pau = PackAlnUnit_alloc();  /* Should deal with memory overflow */ 
        pau->i = i;  
        pau->j = j;  
        pau->state =  state + 2; 
        add_PackAln(out,pau);    
        }  


      max_special_strip_TransRegionMatrix(mat,i,j,state,isspecial,&i,&j,&state,&isspecial,&cellscore);   
      if( i == TransRegionMatrix_READ_OFF_ERROR) {  
        warn("In special strip read TransRegionMatrix, got a bad read off error. Sorry!");   
        return FALSE;    
        }  
      } /* end of while more specials to eat up */ 


    /* check to see we have not gone too far! */ 
    if( state != START && j < TransRegionMatrix_DC_SHADOW_SPECIAL_SP(mat,i,j,state,4))   {  
      warn("In special strip read TransRegionMatrix, at special [%d] state [%d] overshot!",j,state); 
      return FALSE;  
      }  
    /* Put away last state */ 
    if(out != NULL)  {  
      pau = PackAlnUnit_alloc();/* Should deal with memory overflow */ 
      pau->i = i;    
      pau->j = j;    
      pau->state =  state + 2;   
      add_PackAln(out,pau);  
      }  


    /* Put away where we are in startj and startstate */ 
    *startj = j; 
    *startstate = state; 
    return TRUE; 
}    


/* Function:  max_special_strip_TransRegionMatrix(mat,i,j,state,isspecial,reti,retj,retstate,retspecial,cellscore)
 *
 * Descrip:    A pretty intense internal function. Deals with read-off only in specials
 *
 *
 * Arg:               mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:                 i [UNKN ] Undocumented argument [int]
 * Arg:                 j [UNKN ] Undocumented argument [int]
 * Arg:             state [UNKN ] Undocumented argument [int]
 * Arg:         isspecial [UNKN ] Undocumented argument [boolean]
 * Arg:              reti [UNKN ] Undocumented argument [int *]
 * Arg:              retj [UNKN ] Undocumented argument [int *]
 * Arg:          retstate [UNKN ] Undocumented argument [int *]
 * Arg:        retspecial [UNKN ] Undocumented argument [boolean *]
 * Arg:         cellscore [UNKN ] Undocumented argument [int *]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int max_special_strip_TransRegionMatrix(TransRegionMatrix * mat,int i,int j,int state,boolean isspecial,int * reti,int * retj,int * retstate,boolean * retspecial,int * cellscore) 
{
    int temp;    
    int cscore;  


    *reti = (*retj) = (*retstate) = TransRegionMatrix_READ_OFF_ERROR;    
    if( isspecial == FALSE ) {  
      warn("In special strip max function for TransRegionMatrix, got a non special start point. Problem! (bad!)");   
      return (-1);   
      }  


    if( j < 0 || j > mat->seq->len)  {  
      warn("In TransRegionMatrix matrix special read off - out of bounds on matrix [j is %d in special]",j); 
      return -1; 
      }  


    cscore = TransRegionMatrix_DC_SHADOW_SPECIAL(mat,i,j,state); 
    switch(state)    { /*switch on special states*/ 
      case START :   
      case END :     
        /* Source REGION is not a special */ 
        /* Source NON_REGION is not a special */ 
      default:   
        warn("Major problem (!) - in TransRegionMatrix special strip read off, position %d,%d state %d no source found  dropped into default on source switch!",i,j,state);  
        return (-1); 
      } /* end of switch on special states */ 
}    


/* Function:  max_matrix_to_special_TransRegionMatrix(mat,i,j,state,cscore,reti,retj,retstate,retspecial,cellscore)
 *
 * Descrip: No Description
 *
 * Arg:               mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:                 i [UNKN ] Undocumented argument [int]
 * Arg:                 j [UNKN ] Undocumented argument [int]
 * Arg:             state [UNKN ] Undocumented argument [int]
 * Arg:            cscore [UNKN ] Undocumented argument [int]
 * Arg:              reti [UNKN ] Undocumented argument [int *]
 * Arg:              retj [UNKN ] Undocumented argument [int *]
 * Arg:          retstate [UNKN ] Undocumented argument [int *]
 * Arg:        retspecial [UNKN ] Undocumented argument [boolean *]
 * Arg:         cellscore [UNKN ] Undocumented argument [int *]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int max_matrix_to_special_TransRegionMatrix(TransRegionMatrix * mat,int i,int j,int state,int cscore,int * reti,int * retj,int * retstate,boolean * retspecial,int * cellscore) 
{
    int temp;    
    *reti = (*retj) = (*retstate) = TransRegionMatrix_READ_OFF_ERROR;    


    if( j < 0 || j > mat->lenj)  {  
      warn("In TransRegionMatrix matrix to special read off - out of bounds on matrix [j is %d in special]",j);  
      return -1; 
      }  


    switch(state)    { /*Switch state */ 
      case REGION :  
        /* Source NON_REGION is not a special, should not get here! */ 
        temp = cscore - ((TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model))) -  (0);     
        if( temp == TransRegionMatrix_DC_SHADOW_SPECIAL(mat,i - 0,j - 1,START) ) {  
          *reti = i - 0; 
          *retj = j - 1; 
          *retstate = START; 
          *retspecial = TRUE;    
          if( cellscore != NULL) {  
            *cellscore = cscore - TransRegionMatrix_DC_SHADOW_SPECIAL(mat,i-0,j-1,START);    
            }  
          return TransRegionMatrix_DC_SHADOW_MATRIX(mat,i - 0,j - 1,START) ;     
          }  
        /* Source REGION is not a special, should not get here! */ 
        warn("Major problem (!) - in TransRegionMatrix matrix to special read off, position %d,%d state %d no source found!",i,j,state); 
        return (-1); 
      case NON_REGION :  
        /* Source REGION is not a special, should not get here! */ 
        temp = cscore - (TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model)) -  (0);    
        if( temp == TransRegionMatrix_DC_SHADOW_SPECIAL(mat,i - 0,j - 1,START) ) {  
          *reti = i - 0; 
          *retj = j - 1; 
          *retstate = START; 
          *retspecial = TRUE;    
          if( cellscore != NULL) {  
            *cellscore = cscore - TransRegionMatrix_DC_SHADOW_SPECIAL(mat,i-0,j-1,START);    
            }  
          return TransRegionMatrix_DC_SHADOW_MATRIX(mat,i - 0,j - 1,START) ;     
          }  
        /* Source NON_REGION is not a special, should not get here! */ 
        warn("Major problem (!) - in TransRegionMatrix matrix to special read off, position %d,%d state %d no source found!",i,j,state); 
        return (-1); 
      default:   
        warn("Major problem (!) - in TransRegionMatrix read off, position %d,%d state %d no source found!",i,j,state);   
        return (-1); 
      } /* end of Switch state  */ 


}    


/* Function:  calculate_hidden_TransRegionMatrix(mat,starti,startj,startstate,stopi,stopj,stopstate,dpenv)
 *
 * Descrip: No Description
 *
 * Arg:               mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:            starti [UNKN ] Undocumented argument [int]
 * Arg:            startj [UNKN ] Undocumented argument [int]
 * Arg:        startstate [UNKN ] Undocumented argument [int]
 * Arg:             stopi [UNKN ] Undocumented argument [int]
 * Arg:             stopj [UNKN ] Undocumented argument [int]
 * Arg:         stopstate [UNKN ] Undocumented argument [int]
 * Arg:             dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 *
 */
void calculate_hidden_TransRegionMatrix(TransRegionMatrix * mat,int starti,int startj,int startstate,int stopi,int stopj,int stopstate,DPEnvelope * dpenv) 
{
    register int i;  
    register int j;  
    register int score;  
    register int temp;   
    register int hiddenj;    


    hiddenj = startj;    


    init_hidden_TransRegionMatrix(mat,starti,startj,stopi,stopj);    


    TransRegionMatrix_HIDDEN_MATRIX(mat,starti,startj,startstate) = 0;   


    for(j=startj;j<=stopj;j++)   {  
      for(i=starti;i<=stopi;i++) {  
        /* Should *not* do very first cell as this is the one set to zero in one state! */ 
        if( i == starti && j == startj ) 
          continue;  
        if( dpenv != NULL && is_in_DPEnvelope(dpenv,i,j) == FALSE )  { /*Is not in envelope*/ 
          TransRegionMatrix_HIDDEN_MATRIX(mat,i,j,REGION) = NEGI;    
          TransRegionMatrix_HIDDEN_MATRIX(mat,i,j,NON_REGION) = NEGI;    
          continue;  
          } /* end of Is not in envelope */ 


        /* For state REGION */ 
        /* setting first movement to score */ 
        score = TransRegionMatrix_HIDDEN_MATRIX(mat,i-0,j-1,REGION) + (TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model));    
        /* From state NON_REGION to state REGION */ 
        temp = TransRegionMatrix_HIDDEN_MATRIX(mat,i-0,j-1,NON_REGION) + mat->model->region_start;   
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for REGION */ 
        /* Add any movement independant score and put away */ 
         TransRegionMatrix_HIDDEN_MATRIX(mat,i,j,REGION) = score;    
        /* Finished calculating state REGION */ 


        /* For state NON_REGION */ 
        /* setting first movement to score */ 
        score = TransRegionMatrix_HIDDEN_MATRIX(mat,i-0,j-1,NON_REGION) + TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model);   
        /* From state REGION to state NON_REGION */ 
        temp = TransRegionMatrix_HIDDEN_MATRIX(mat,i-0,j-1,REGION) + 0;  
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for NON_REGION */ 
        /* Add any movement independant score and put away */ 
         TransRegionMatrix_HIDDEN_MATRIX(mat,i,j,NON_REGION) = score;    
        /* Finished calculating state NON_REGION */ 
        }  
      }  


    return;  
}    


/* Function:  init_hidden_TransRegionMatrix(mat,starti,startj,stopi,stopj)
 *
 * Descrip: No Description
 *
 * Arg:           mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:        starti [UNKN ] Undocumented argument [int]
 * Arg:        startj [UNKN ] Undocumented argument [int]
 * Arg:         stopi [UNKN ] Undocumented argument [int]
 * Arg:         stopj [UNKN ] Undocumented argument [int]
 *
 */
void init_hidden_TransRegionMatrix(TransRegionMatrix * mat,int starti,int startj,int stopi,int stopj) 
{
    register int i;  
    register int j;  
    register int hiddenj;    


    hiddenj = startj;    
    for(j=(startj-1);j<=stopj;j++)   {  
      for(i=(starti-0);i<=stopi;i++) {  
        TransRegionMatrix_HIDDEN_MATRIX(mat,i,j,REGION) = NEGI;
 
        TransRegionMatrix_HIDDEN_MATRIX(mat,i,j,NON_REGION) = NEGI;
 
        }  
      }  


    return;  
}    


/* Function:  full_dc_TransRegionMatrix(mat,starti,startj,startstate,stopi,stopj,stopstate,out,donej,totalj,dpenv)
 *
 * Descrip:    The main divide-and-conquor routine. Basically, call /PackAln_calculate_small_TransRegionMatrix
 *             Not this function, which is pretty hard core. 
 *             Function is given start/end points (in main matrix) for alignment
 *             It does some checks, decides whether start/end in j is small enough for explicit calc
 *               - if yes, calculates it, reads off into PackAln (out), adds the j distance to donej and returns TRUE
 *               - if no,  uses /do_dc_single_pass_TransRegionMatrix to get mid-point
 *                          saves midpoint, and calls itself to do right portion then left portion
 *             right then left ensures PackAln is added the 'right' way, ie, back-to-front
 *             returns FALSE on any error, with a warning
 *
 *
 * Arg:               mat [UNKN ] Matrix with small memory implementation [TransRegionMatrix *]
 * Arg:            starti [UNKN ] Start position in i [int]
 * Arg:            startj [UNKN ] Start position in j [int]
 * Arg:        startstate [UNKN ] Start position state number [int]
 * Arg:             stopi [UNKN ] Stop position in i [int]
 * Arg:             stopj [UNKN ] Stop position in j [int]
 * Arg:         stopstate [UNKN ] Stop position state number [int]
 * Arg:               out [UNKN ] PackAln structure to put alignment into [PackAln *]
 * Arg:             donej [UNKN ] pointer to a number with the amount of alignment done [int *]
 * Arg:            totalj [UNKN ] total amount of alignment to do (in j coordinates) [int]
 * Arg:             dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
boolean full_dc_TransRegionMatrix(TransRegionMatrix * mat,int starti,int startj,int startstate,int stopi,int stopj,int stopstate,PackAln * out,int * donej,int totalj,DPEnvelope * dpenv) 
{
    int lstarti; 
    int lstartj; 
    int lstate;  


    if( mat->basematrix->type != BASEMATRIX_TYPE_SHADOW) {  
      warn("*Very* bad error! - non shadow matrix type in full_dc_TransRegionMatrix");   
      return FALSE;  
      }  


    if( starti == -1 || startj == -1 || startstate == -1 || stopi == -1 || stopstate == -1)  {  
      warn("In full dc program, passed bad indices, indices passed were %d:%d[%d] to %d:%d[%d]\n",starti,startj,startstate,stopi,stopj,stopstate);   
      return FALSE;  
      }  


    if( stopj - startj < 5)  {  
      log_full_error(REPORT,0,"[%d,%d][%d,%d] Explicit read off",starti,startj,stopi,stopj);/* Build hidden explicit matrix */ 
      calculate_hidden_TransRegionMatrix(mat,starti,startj,startstate,stopi,stopj,stopstate,dpenv);  
      *donej += (stopj - startj);   /* Now read it off into out */ 
      if( read_hidden_TransRegionMatrix(mat,starti,startj,startstate,stopi,stopj,stopstate,out) == FALSE)    {  
        warn("In full dc, at %d:%d,%d:%d got a bad hidden explicit read off... ",starti,startj,stopi,stopj); 
        return FALSE;    
        }  
      return TRUE;   
      }  


/* In actual divide and conquor */ 
    if( do_dc_single_pass_TransRegionMatrix(mat,starti,startj,startstate,stopi,stopj,stopstate,dpenv,(int)(*donej*100)/totalj) == FALSE) {  
      warn("In divide and conquor for TransRegionMatrix, at bound %d:%d to %d:%d, unable to calculate midpoint. Problem!",starti,startj,stopi,stopj);    
      return FALSE;  
      }  


/* Ok... now we have to call on each side of the matrix */ 
/* We have to retrieve left hand side positions, as they will be vapped by the time we call LHS */ 
    lstarti= TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,stopi,stopj,stopstate,0);     
    lstartj= TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,stopi,stopj,stopstate,1);     
    lstate = TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,stopi,stopj,stopstate,2);     


/* Call on right hand side: this lets us do the correct read off */ 
    if( full_dc_TransRegionMatrix(mat,TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,stopi,stopj,stopstate,3),TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,stopi,stopj,stopstate,4),TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,stopi,stopj,stopstate,5),stopi,stopj,stopstate,out,donej,totalj,dpenv) == FALSE)   {  
/* Warning already issued, simply chained back up to top */ 
      return FALSE;  
      }  
/* Call on left hand side */ 
    if( full_dc_TransRegionMatrix(mat,starti,startj,startstate,lstarti,lstartj,lstate,out,donej,totalj,dpenv) == FALSE)  {  
/* Warning already issued, simply chained back up to top */ 
      return FALSE;  
      }  


    return TRUE;     
}    


/* Function:  do_dc_single_pass_TransRegionMatrix(mat,starti,startj,startstate,stopi,stopj,stopstate,dpenv,perc_done)
 *
 * Descrip: No Description
 *
 * Arg:               mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:            starti [UNKN ] Undocumented argument [int]
 * Arg:            startj [UNKN ] Undocumented argument [int]
 * Arg:        startstate [UNKN ] Undocumented argument [int]
 * Arg:             stopi [UNKN ] Undocumented argument [int]
 * Arg:             stopj [UNKN ] Undocumented argument [int]
 * Arg:         stopstate [UNKN ] Undocumented argument [int]
 * Arg:             dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 * Arg:         perc_done [UNKN ] Undocumented argument [int]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
boolean do_dc_single_pass_TransRegionMatrix(TransRegionMatrix * mat,int starti,int startj,int startstate,int stopi,int stopj,int stopstate,DPEnvelope * dpenv,int perc_done) 
{
    int halfj;   
    halfj = startj + ((stopj - startj)/2);   


    init_dc_TransRegionMatrix(mat);  


    TransRegionMatrix_DC_SHADOW_MATRIX(mat,starti,startj,startstate) = 0;    
    run_up_dc_TransRegionMatrix(mat,starti,stopi,startj,halfj-1,dpenv,perc_done);    
    push_dc_at_merge_TransRegionMatrix(mat,starti,stopi,halfj,&halfj,dpenv);     
    follow_on_dc_TransRegionMatrix(mat,starti,stopi,halfj,stopj,dpenv,perc_done);    
    return TRUE; 
}    


/* Function:  push_dc_at_merge_TransRegionMatrix(mat,starti,stopi,startj,stopj,dpenv)
 *
 * Descrip: No Description
 *
 * Arg:           mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:        starti [UNKN ] Undocumented argument [int]
 * Arg:         stopi [UNKN ] Undocumented argument [int]
 * Arg:        startj [UNKN ] Undocumented argument [int]
 * Arg:         stopj [UNKN ] Undocumented argument [int *]
 * Arg:         dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 *
 */
void push_dc_at_merge_TransRegionMatrix(TransRegionMatrix * mat,int starti,int stopi,int startj,int * stopj,DPEnvelope * dpenv) 
{
    register int i;  
    register int j;  
    register int k;  
    register int count;  
    register int mergej;/* Sources below this j will be stamped by triples */ 
    register int score;  
    register int temp;   


    mergej = startj -1;  
    for(count=0,j=startj;count<1;count++,j++)    {  
      for(i=starti;i<=stopi;i++) {  
        if( dpenv != NULL && is_in_DPEnvelope(dpenv,i,j) == FALSE )  { /*Is not in envelope*/ 
          TransRegionMatrix_DC_SHADOW_MATRIX(mat,i,j,REGION) = NEGI;     
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,REGION,0) = (-100);  
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,REGION,1) = (-100);  
          TransRegionMatrix_DC_SHADOW_MATRIX(mat,i,j,NON_REGION) = NEGI;     
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,0) = (-100);  
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,1) = (-100);  
          continue;  
          } /* end of Is not in envelope */ 


        /* For state REGION, pushing when j - offj <= mergej */ 
        score = TransRegionMatrix_DC_SHADOW_MATRIX(mat,i-0,j-1,REGION) + (TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model));     
        if( j - 1 <= mergej) {  
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,REGION,0) = i-0; 
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,REGION,1) = j-1; 
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,REGION,2) = REGION;  
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,REGION,3) = i;   
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,REGION,4) = j;   
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,REGION,5) = REGION;  
          }  
        else {  
          for(k=0;k<7;k++)   
            TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,REGION,k) = TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i - 0,j - 1,REGION,k);   
          }  


        temp = TransRegionMatrix_DC_SHADOW_MATRIX(mat,i-0,j-1,NON_REGION) + mat->model->region_start;    
        if( temp > score)    {  
          score = temp;  


          if( j - 1 <= mergej)   {  
            TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,REGION,0) = i-0;   
            TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,REGION,1) = j-1;   
            TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,REGION,2) = NON_REGION;    
            TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,REGION,3) = i; 
            TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,REGION,4) = j; 
            TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,REGION,5) = REGION;    
            }  
          else   {  
            for(k=0;k<7;k++) 
              TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,REGION,k) = TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i - 0,j - 1,NON_REGION,k); 
            }  
          }  
        /* Add any movement independant score */ 
        TransRegionMatrix_DC_SHADOW_MATRIX(mat,i,j,REGION) = score;  
        /* Finished with state REGION */ 


        /* For state NON_REGION, pushing when j - offj <= mergej */ 
        score = TransRegionMatrix_DC_SHADOW_MATRIX(mat,i-0,j-1,NON_REGION) + TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model);    
        if( j - 1 <= mergej) {  
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,0) = i-0; 
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,1) = j-1; 
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,2) = NON_REGION;  
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,3) = i;   
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,4) = j;   
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,5) = NON_REGION;  
          }  
        else {  
          for(k=0;k<7;k++)   
            TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,k) = TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i - 0,j - 1,NON_REGION,k);   
          }  


        temp = TransRegionMatrix_DC_SHADOW_MATRIX(mat,i-0,j-1,REGION) + 0;   
        if( temp > score)    {  
          score = temp;  


          if( j - 1 <= mergej)   {  
            TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,0) = i-0;   
            TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,1) = j-1;   
            TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,2) = REGION;    
            TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,3) = i; 
            TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,4) = j; 
            TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,5) = NON_REGION;    
            }  
          else   {  
            for(k=0;k<7;k++) 
              TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,k) = TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i - 0,j - 1,REGION,k); 
            }  
          }  
        /* Add any movement independant score */ 
        TransRegionMatrix_DC_SHADOW_MATRIX(mat,i,j,NON_REGION) = score;  
        /* Finished with state NON_REGION */ 
        }  
      }  
    /* Put back j into * stop j so that calling function gets it correct */ 
    if( stopj == NULL)   
      warn("Bad news... NULL stopj pointer in push dc function. This means that calling function does not know how many cells I have done!");    
    else 
      *stopj = j;    


    return;  
}    


/* Function:  follow_on_dc_TransRegionMatrix(mat,starti,stopi,startj,stopj,dpenv,perc_done)
 *
 * Descrip: No Description
 *
 * Arg:              mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:           starti [UNKN ] Undocumented argument [int]
 * Arg:            stopi [UNKN ] Undocumented argument [int]
 * Arg:           startj [UNKN ] Undocumented argument [int]
 * Arg:            stopj [UNKN ] Undocumented argument [int]
 * Arg:            dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 * Arg:        perc_done [UNKN ] Undocumented argument [int]
 *
 */
void follow_on_dc_TransRegionMatrix(TransRegionMatrix * mat,int starti,int stopi,int startj,int stopj,DPEnvelope * dpenv,int perc_done) 
{
    int i;   
    int j;   
    int k;   
    int score;   
    int temp;    
    int localshadow[7];  
    long int total;  
    long int num;    


    total = (stopi - starti+1) * (stopj - startj+1); 
    num = 0;     


    for(j=startj;j<=stopj;j++)   { /*for each valid j column*/ 
      for(i=starti;i<=stopi;i++) { /*this is strip*/ 
        num++;   
        if( dpenv != NULL && is_in_DPEnvelope(dpenv,i,j) == FALSE )  { /*Is not in envelope*/ 
          TransRegionMatrix_DC_SHADOW_MATRIX(mat,i,j,REGION) = NEGI;     
          TransRegionMatrix_DC_SHADOW_MATRIX(mat,i,j,NON_REGION) = NEGI;     
          continue;  
          } /* end of Is not in envelope */ 
        if( num % 1000 == 0 )    
          log_full_error(REPORT,0,"[%d%%%% done]After  mid-j %5d Cells done %d%%%%",perc_done,startj,(num*100)/total);   


        /* For state REGION */ 
        /* setting first movement to score */ 
        score = TransRegionMatrix_DC_SHADOW_MATRIX(mat,i-0,j-1,REGION) + (TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model));     
        /* shift first shadow numbers */ 
        for(k=0;k<7;k++) 
          localshadow[k] = TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i - 0,j - 1,REGION,k);  
        /* From state NON_REGION to state REGION */ 
        temp = TransRegionMatrix_DC_SHADOW_MATRIX(mat,i-0,j-1,NON_REGION) + mat->model->region_start;    
        if( temp  > score )  {  
          score = temp;  
          for(k=0;k<7;k++)   
            localshadow[k] = TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i - 0,j - 1,NON_REGION,k);    
          }  


        /* Ok - finished max calculation for REGION */ 
        /* Add any movement independant score and put away */ 
         TransRegionMatrix_DC_SHADOW_MATRIX(mat,i,j,REGION) = score; 
        for(k=0;k<7;k++) 
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,REGION,k) = localshadow[k];  
        /* Now figure out if any specials need this score */ 
        /* Finished calculating state REGION */ 


        /* For state NON_REGION */ 
        /* setting first movement to score */ 
        score = TransRegionMatrix_DC_SHADOW_MATRIX(mat,i-0,j-1,NON_REGION) + TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model);    
        /* shift first shadow numbers */ 
        for(k=0;k<7;k++) 
          localshadow[k] = TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i - 0,j - 1,NON_REGION,k);  
        /* From state REGION to state NON_REGION */ 
        temp = TransRegionMatrix_DC_SHADOW_MATRIX(mat,i-0,j-1,REGION) + 0;   
        if( temp  > score )  {  
          score = temp;  
          for(k=0;k<7;k++)   
            localshadow[k] = TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i - 0,j - 1,REGION,k);    
          }  


        /* Ok - finished max calculation for NON_REGION */ 
        /* Add any movement independant score and put away */ 
         TransRegionMatrix_DC_SHADOW_MATRIX(mat,i,j,NON_REGION) = score; 
        for(k=0;k<7;k++) 
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,k) = localshadow[k];  
        /* Now figure out if any specials need this score */ 
        /* Finished calculating state NON_REGION */ 
        } /* end of this is strip */ 
      } /* end of for each valid j column */ 


/* Function:  run_up_dc_TransRegionMatrix(mat,starti,stopi,startj,stopj,dpenv,perc_done)
 *
 * Descrip: No Description
 *
 * Arg:              mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:           starti [UNKN ] Undocumented argument [int]
 * Arg:            stopi [UNKN ] Undocumented argument [int]
 * Arg:           startj [UNKN ] Undocumented argument [int]
 * Arg:            stopj [UNKN ] Undocumented argument [int]
 * Arg:            dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 * Arg:        perc_done [UNKN ] Undocumented argument [int]
 *
 */
}    
void run_up_dc_TransRegionMatrix(TransRegionMatrix * mat,int starti,int stopi,int startj,int stopj,DPEnvelope * dpenv,int perc_done) 
{
    register int i;  
    register int j;  
    register int score;  
    register int temp;   
    long int total;  
    long int num;    


    total = (stopi - starti+1) * (stopj - startj+1); 
    if( total <= 0 ) 
      total = 1; 
    num = 0;     


    for(j=startj;j<=stopj;j++)   { /*for each valid j column*/ 
      for(i=starti;i<=stopi;i++) { /*this is strip*/ 
        if( j == startj && i == starti)  
          continue;  
        num++;   
        if( dpenv != NULL && is_in_DPEnvelope(dpenv,i,j) == FALSE )  { /*Is not in envelope*/ 
          TransRegionMatrix_DC_SHADOW_MATRIX(mat,i,j,REGION) = NEGI;     
          TransRegionMatrix_DC_SHADOW_MATRIX(mat,i,j,NON_REGION) = NEGI;     
          continue;  
          } /* end of Is not in envelope */ 
        if( num % 1000 == 0 )    
          log_full_error(REPORT,0,"[%d%%%% done]Before mid-j %5d Cells done %d%%%%",perc_done,stopj,(num*100)/total);    


        /* For state REGION */ 
        /* setting first movement to score */ 
        score = TransRegionMatrix_DC_SHADOW_MATRIX(mat,i-0,j-1,REGION) + (TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model));     
        /* From state NON_REGION to state REGION */ 
        temp = TransRegionMatrix_DC_SHADOW_MATRIX(mat,i-0,j-1,NON_REGION) + mat->model->region_start;    
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for REGION */ 
        /* Add any movement independant score and put away */ 
         TransRegionMatrix_DC_SHADOW_MATRIX(mat,i,j,REGION) = score; 
        /* Finished calculating state REGION */ 


        /* For state NON_REGION */ 
        /* setting first movement to score */ 
        score = TransRegionMatrix_DC_SHADOW_MATRIX(mat,i-0,j-1,NON_REGION) + TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model);    
        /* From state REGION to state NON_REGION */ 
        temp = TransRegionMatrix_DC_SHADOW_MATRIX(mat,i-0,j-1,REGION) + 0;   
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for NON_REGION */ 
        /* Add any movement independant score and put away */ 
         TransRegionMatrix_DC_SHADOW_MATRIX(mat,i,j,NON_REGION) = score; 
        /* Finished calculating state NON_REGION */ 
        } /* end of this is strip */ 
      } /* end of for each valid j column */ 


/* Function:  init_dc_TransRegionMatrix(mat)
 *
 * Descrip: No Description
 *
 * Arg:        mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 *
 */
}    
void init_dc_TransRegionMatrix(TransRegionMatrix * mat) 
{
    register int i;  
    register int j;  
    register int k;  


    for(j=0;j<3;j++) {  
      for(i=(-0);i<mat->model->len;i++)  {  
        TransRegionMatrix_DC_SHADOW_MATRIX(mat,i,j,REGION) = NEGI;   
        TransRegionMatrix_DC_SHADOW_MATRIX(mat,i,j,NON_REGION) = NEGI;   
        for(k=0;k<7;k++) {  
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,REGION,k) = (-1);    
          TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,k) = (-1);    
          }  
        }  
      }  


    return;  
}    


/* Function:  start_end_find_end_TransRegionMatrix(mat,endj)
 *
 * Descrip:    First function used to find end of the best path in the special state !end
 *
 *
 * Arg:         mat [UNKN ] Matrix in small mode [TransRegionMatrix *]
 * Arg:        endj [WRITE] position of end in j (meaningless in i) [int *]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int start_end_find_end_TransRegionMatrix(TransRegionMatrix * mat,int * endj) 
{
    register int j;  
    register int max;    
    register int maxj;   


    max = TransRegionMatrix_DC_SHADOW_SPECIAL(mat,0,mat->seq->len-1,END);    
    maxj = mat->seq->len-1;  
    for(j= mat->seq->len-2 ;j >= 0 ;j--) {  
      if( TransRegionMatrix_DC_SHADOW_SPECIAL(mat,0,j,END) > max )   {  
        max = TransRegionMatrix_DC_SHADOW_SPECIAL(mat,0,j,END);  
        maxj = j;    
        }  
      }  


    if( endj != NULL)    
      *endj = maxj;  


    return max;  
}    


/* Function:  dc_optimised_start_end_calc_TransRegionMatrix(*mat,dpenv)
 *
 * Descrip:    Calculates special strip, leaving start/end/score points in shadow matrix
 *             Works off specially laid out memory from steve searle
 *
 *
 * Arg:         *mat [UNKN ] Undocumented argument [TransRegionMatrix]
 * Arg:        dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
boolean dc_optimised_start_end_calc_TransRegionMatrix(TransRegionMatrix *mat,DPEnvelope * dpenv) 
{
    int i;   
    int j;   
    int k;   
    int score;   
    int temp;    
    int leni;    
    int lenj;    
    int localshadow[7];  
    long int total;  
    long int num=0;  
    int * score_pointers;    
    int * shadow_pointers;   
    int * localsp;   
    leni = mat->model->len;  
    lenj = mat->seq->len;    
    total = leni * lenj; 


    score_pointers = (int *) calloc (1 * (leni + 0) * 2,sizeof(int));    
    shadow_pointers = (int *) calloc (1 * (leni + 0) * 2 * 8,sizeof(int));   


    for(j=0;j<lenj;j++)  { /*for each j strip*/ 
      for(i=0;i<leni;i++)    { /*for each i position in strip*/ 
        num++;   
        if( dpenv != NULL && is_in_DPEnvelope(dpenv,i,j) == FALSE )  { /*Is not in envelope*/ 
          TransRegionMatrix_DC_OPT_SHADOW_MATRIX(mat,i,j,REGION) = NEGI;     
          TransRegionMatrix_DC_OPT_SHADOW_MATRIX(mat,i,j,NON_REGION) = NEGI;     
          continue;  
          } /* end of Is not in envelope */ 
        if( num%1000 == 0)   
          log_full_error(REPORT,0,"%6d Cells done [%2d%%%%]",num,num*100/total); 




        /* For state REGION */ 
        /* setting first movement to score */ 
        score = TransRegionMatrix_DC_OPT_SHADOW_MATRIX(mat,i-0,j-1,REGION) + (TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model)) + (0);   
        /* assign local shadown pointer */ 
        localsp = &(TransRegionMatrix_DC_OPT_SHADOW_MATRIX_SP(mat,i - 0,j - 1,REGION,0));    
        /* From state START to state REGION */ 
        temp = TransRegionMatrix_DC_OPT_SHADOW_SPECIAL(mat,i-0,j-1,START) + (TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model)) + (0);    
        if( temp  > score )  {  
          score = temp;  
          /* This state [START] is a special for REGION... push top shadow pointers here */ 
          localshadow[0]= i; 
          localshadow[1]= j; 
          localshadow[2]= REGION;    
          localshadow[3]= (-1);  
          localshadow[4]= (-1);  
          localshadow[5]= (-1);  
          localshadow[6]= score; 
          localsp = localshadow; 
          }  
        /* From state NON_REGION to state REGION */ 
        temp = TransRegionMatrix_DC_OPT_SHADOW_MATRIX(mat,i-0,j-1,NON_REGION) + mat->model->region_start +(0);   
        if( temp  > score )  {  
          score = temp;  
          /* assign local shadown pointer */ 
          localsp = &(TransRegionMatrix_DC_OPT_SHADOW_MATRIX_SP(mat,i - 0,j - 1,NON_REGION,0));  
          }  


        /* Ok - finished max calculation for REGION */ 
        /* Add any movement independant score and put away */ 
        /* Actually, already done inside scores */ 
         TransRegionMatrix_DC_OPT_SHADOW_MATRIX(mat,i,j,REGION) = score; 
        for(k=0;k<7;k++) 
          TransRegionMatrix_DC_OPT_SHADOW_MATRIX_SP(mat,i,j,REGION,k) = localsp[k];  
        /* Now figure out if any specials need this score */ 


        /* state REGION is a source for special END */ 
        temp = score + (0) + (0) ;   
        if( temp > TransRegionMatrix_DC_OPT_SHADOW_SPECIAL(mat,i,j,END) )    {  
          TransRegionMatrix_DC_OPT_SHADOW_SPECIAL(mat,i,j,END) = temp;   
          /* Have to push only bottem half of system here */ 
          for(k=0;k<3;k++)   
            TransRegionMatrix_DC_OPT_SHADOW_SPECIAL_SP(mat,i,j,END,k) = TransRegionMatrix_DC_OPT_SHADOW_MATRIX_SP(mat,i,j,REGION,k); 
          TransRegionMatrix_DC_OPT_SHADOW_SPECIAL_SP(mat,i,j,END,6) = TransRegionMatrix_DC_OPT_SHADOW_MATRIX_SP(mat,i,j,REGION,6);   
          TransRegionMatrix_DC_OPT_SHADOW_SPECIAL_SP(mat,i,j,END,3) = i; 
          TransRegionMatrix_DC_OPT_SHADOW_SPECIAL_SP(mat,i,j,END,4) = j; 
          TransRegionMatrix_DC_OPT_SHADOW_SPECIAL_SP(mat,i,j,END,5) = REGION;    
          }  




        /* Finished calculating state REGION */ 


        /* For state NON_REGION */ 
        /* setting first movement to score */ 
        score = TransRegionMatrix_DC_OPT_SHADOW_MATRIX(mat,i-0,j-1,NON_REGION) + TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model) + (0);  
        /* assign local shadown pointer */ 
        localsp = &(TransRegionMatrix_DC_OPT_SHADOW_MATRIX_SP(mat,i - 0,j - 1,NON_REGION,0));    
        /* From state START to state NON_REGION */ 
        temp = TransRegionMatrix_DC_OPT_SHADOW_SPECIAL(mat,i-0,j-1,START) + TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model) + (0);   
        if( temp  > score )  {  
          score = temp;  
          /* This state [START] is a special for NON_REGION... push top shadow pointers here */ 
          localshadow[0]= i; 
          localshadow[1]= j; 
          localshadow[2]= NON_REGION;    
          localshadow[3]= (-1);  
          localshadow[4]= (-1);  
          localshadow[5]= (-1);  
          localshadow[6]= score; 
          localsp = localshadow; 
          }  
        /* From state REGION to state NON_REGION */ 
        temp = TransRegionMatrix_DC_OPT_SHADOW_MATRIX(mat,i-0,j-1,REGION) + 0 +(0);  
        if( temp  > score )  {  
          score = temp;  
          /* assign local shadown pointer */ 
          localsp = &(TransRegionMatrix_DC_OPT_SHADOW_MATRIX_SP(mat,i - 0,j - 1,REGION,0));  
          }  


        /* Ok - finished max calculation for NON_REGION */ 
        /* Add any movement independant score and put away */ 
        /* Actually, already done inside scores */ 
         TransRegionMatrix_DC_OPT_SHADOW_MATRIX(mat,i,j,NON_REGION) = score; 
        for(k=0;k<7;k++) 
          TransRegionMatrix_DC_OPT_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,k) = localsp[k];  
        /* Now figure out if any specials need this score */ 


        /* state NON_REGION is a source for special END */ 
        temp = score + (0) + (0) ;   
        if( temp > TransRegionMatrix_DC_OPT_SHADOW_SPECIAL(mat,i,j,END) )    {  
          TransRegionMatrix_DC_OPT_SHADOW_SPECIAL(mat,i,j,END) = temp;   
          /* Have to push only bottem half of system here */ 
          for(k=0;k<3;k++)   
            TransRegionMatrix_DC_OPT_SHADOW_SPECIAL_SP(mat,i,j,END,k) = TransRegionMatrix_DC_OPT_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,k); 
          TransRegionMatrix_DC_OPT_SHADOW_SPECIAL_SP(mat,i,j,END,6) = TransRegionMatrix_DC_OPT_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,6);   
          TransRegionMatrix_DC_OPT_SHADOW_SPECIAL_SP(mat,i,j,END,3) = i; 
          TransRegionMatrix_DC_OPT_SHADOW_SPECIAL_SP(mat,i,j,END,4) = j; 
          TransRegionMatrix_DC_OPT_SHADOW_SPECIAL_SP(mat,i,j,END,5) = NON_REGION;    
          }  




        /* Finished calculating state NON_REGION */ 


        } /* end of for each i position in strip */ 
      } /* end of for each j strip */ 
    free(score_pointers);    
    free(shadow_pointers);   
    return TRUE;     
}    


/* Function:  init_start_end_linear_TransRegionMatrix(mat)
 *
 * Descrip: No Description
 *
 * Arg:        mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 *
 */
void init_start_end_linear_TransRegionMatrix(TransRegionMatrix * mat) 
{
    register int i;  
    register int j;  
    for(j=0;j<3;j++) {  
      for(i=(-0);i<mat->model->len;i++)  {  
        TransRegionMatrix_DC_SHADOW_MATRIX(mat,i,j,REGION) = NEGI;   
        TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,REGION,0) = (-1);  
        TransRegionMatrix_DC_SHADOW_MATRIX(mat,i,j,NON_REGION) = NEGI;   
        TransRegionMatrix_DC_SHADOW_MATRIX_SP(mat,i,j,NON_REGION,0) = (-1);  
        }  
      }  


    for(j=(-1);j<mat->seq->len;j++)  {  
      TransRegionMatrix_DC_SHADOW_SPECIAL(mat,0,j,START) = 0;    
      TransRegionMatrix_DC_SHADOW_SPECIAL_SP(mat,0,j,START,0) = j;   
      TransRegionMatrix_DC_SHADOW_SPECIAL(mat,0,j,END) = NEGI;   
      TransRegionMatrix_DC_SHADOW_SPECIAL_SP(mat,0,j,END,0) = (-1);  
      }  


    return;  
}    


/* Function:  convert_PackAln_to_AlnBlock_TransRegionMatrix(pal)
 *
 * Descrip:    Converts a path alignment to a label alignment
 *             The label alignment is probably much more useful than the path
 *
 *
 * Arg:        pal [UNKN ] Undocumented argument [PackAln *]
 *
 * Return [UNKN ]  Undocumented return value [AlnBlock *]
 *
 */
AlnBlock * convert_PackAln_to_AlnBlock_TransRegionMatrix(PackAln * pal) 
{
    AlnConvertSet * acs; 
    AlnBlock * alb;  


    acs = AlnConvertSet_TransRegionMatrix(); 
    alb = AlnBlock_from_PackAln(acs,pal);    
    free_AlnConvertSet(acs); 
    return alb;  
}    


 static char * query_label[] = { "REGION_STATE","NON_REGION_STATE","END" };  
/* Function:  AlnConvertSet_TransRegionMatrix(void)
 *
 * Descrip: No Description
 *
 *
 * Return [UNKN ]  Undocumented return value [AlnConvertSet *]
 *
 */
 static char * target_label[] = { "REGION","RANDOM","END" }; 
AlnConvertSet * AlnConvertSet_TransRegionMatrix(void) 
{
    AlnConvertUnit * acu;    
    AlnConvertSet  * out;    


    out = AlnConvertSet_alloc_std(); 


    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = REGION;    
    acu->state2 = REGION;    
    acu->offi = 0;   
    acu->offj = 1;   
    acu->label1 = query_label[0];    
    acu->label2 = target_label[0];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = START + 2; 
    acu->is_from_special = TRUE; 
    acu->state2 = REGION;    
    acu->offi = (-1);    
    acu->offj = 1;   
    acu->label1 = query_label[0];    
    acu->label2 = target_label[0];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = NON_REGION;    
    acu->state2 = REGION;    
    acu->offi = 0;   
    acu->offj = 1;   
    acu->label1 = query_label[0];    
    acu->label2 = target_label[0];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = NON_REGION;    
    acu->state2 = NON_REGION;    
    acu->offi = 0;   
    acu->offj = 1;   
    acu->label1 = query_label[1];    
    acu->label2 = target_label[1];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = START + 2; 
    acu->is_from_special = TRUE; 
    acu->state2 = NON_REGION;    
    acu->offi = (-1);    
    acu->offj = 1;   
    acu->label1 = query_label[1];    
    acu->label2 = target_label[1];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = REGION;    
    acu->state2 = NON_REGION;    
    acu->offi = 0;   
    acu->offj = 1;   
    acu->label1 = query_label[1];    
    acu->label2 = target_label[1];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = NON_REGION;    
    acu->state2 = END + 2;   
    acu->offi = (-1);    
    acu->offj = 0;   
    acu->label1 = query_label[2];    
    acu->label2 = target_label[2];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = REGION;    
    acu->state2 = END + 2;   
    acu->offi = (-1);    
    acu->offj = 0;   
    acu->label1 = query_label[2];    
    acu->label2 = target_label[2];   
    add_collapse_label_AlnConvertSet(out,"NON_REGION_STATE","RANDOM");   
    add_collapse_label_AlnConvertSet(out,"REGION_STATE","REGION");   
    return out;  
}    


/* Function:  PackAln_read_Expl_TransRegionMatrix(mat)
 *
 * Descrip:    Reads off PackAln from explicit matrix structure
 *
 *
 * Arg:        mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 *
 * Return [UNKN ]  Undocumented return value [PackAln *]
 *
 */
PackAln * PackAln_read_Expl_TransRegionMatrix(TransRegionMatrix * mat) 
{
    TransRegionMatrix_access_func_holder holder;     


    holder.access_main    = TransRegionMatrix_explicit_access_main;  
    holder.access_special = TransRegionMatrix_explicit_access_special;   
    return PackAln_read_generic_TransRegionMatrix(mat,holder);   
}    


/* Function:  TransRegionMatrix_explicit_access_main(mat,i,j,state)
 *
 * Descrip: No Description
 *
 * Arg:          mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:            i [UNKN ] Undocumented argument [int]
 * Arg:            j [UNKN ] Undocumented argument [int]
 * Arg:        state [UNKN ] Undocumented argument [int]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int TransRegionMatrix_explicit_access_main(TransRegionMatrix * mat,int i,int j,int state) 
{
    return TransRegionMatrix_EXPL_MATRIX(mat,i,j,state); 
}    


/* Function:  TransRegionMatrix_explicit_access_special(mat,i,j,state)
 *
 * Descrip: No Description
 *
 * Arg:          mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:            i [UNKN ] Undocumented argument [int]
 * Arg:            j [UNKN ] Undocumented argument [int]
 * Arg:        state [UNKN ] Undocumented argument [int]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int TransRegionMatrix_explicit_access_special(TransRegionMatrix * mat,int i,int j,int state) 
{
    return TransRegionMatrix_EXPL_SPECIAL(mat,i,j,state);    
}    


/* Function:  PackAln_read_generic_TransRegionMatrix(mat,h)
 *
 * Descrip:    Reads off PackAln from explicit matrix structure
 *
 *
 * Arg:        mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:          h [UNKN ] Undocumented argument [TransRegionMatrix_access_func_holder]
 *
 * Return [UNKN ]  Undocumented return value [PackAln *]
 *
 */
PackAln * PackAln_read_generic_TransRegionMatrix(TransRegionMatrix * mat,TransRegionMatrix_access_func_holder h) 
{
    register PackAln * out;  
    int i;   
    int j;   
    int state;   
    int cellscore = (-1);    
    boolean isspecial;   
    PackAlnUnit * pau = NULL;    
    PackAlnUnit * prev = NULL;   


    assert(mat);     
    assert(h.access_main);   
    assert(h.access_special);    


    out = PackAln_alloc_std();   
    if( out == NULL )    
      return NULL;   


    out->score =  find_end_TransRegionMatrix(mat,&i,&j,&state,&isspecial,h); 


    /* Add final end transition (at the moment we have not got the score! */ 
    if( (pau= PackAlnUnit_alloc()) == NULL  || add_PackAln(out,pau) == FALSE )   {  
      warn("Failed the first PackAlnUnit alloc, %d length of Alignment in TransRegionMatrix_basic_read, returning a mess.(Sorry!)",out->len);    
      return out;    
      }  


    /* Put in positions for end trans. Remember that coordinates in C style */ 
    pau->i = i;  
    pau->j = j;  
    if( isspecial != TRUE)   
      pau->state = state;    
    else pau->state = state + 2;     
    prev=pau;    
    while( state != START || isspecial != TRUE)  { /*while state != START*/ 


      if( isspecial == TRUE )    
        max_calc_special_TransRegionMatrix(mat,i,j,state,isspecial,&i,&j,&state,&isspecial,&cellscore,h);    
      else   
        max_calc_TransRegionMatrix(mat,i,j,state,isspecial,&i,&j,&state,&isspecial,&cellscore,h);    
      if(i == TransRegionMatrix_READ_OFF_ERROR || j == TransRegionMatrix_READ_OFF_ERROR || state == TransRegionMatrix_READ_OFF_ERROR )   {  
        warn("Problem - hit bad read off system, exiting now");  
        break;   
        }  
      if( (pau= PackAlnUnit_alloc()) == NULL  || add_PackAln(out,pau) == FALSE ) {  
        warn("Failed a PackAlnUnit alloc, %d length of Alignment in TransRegionMatrix_basic_read, returning partial alignment",out->len);    
        break;   
        }  


      /* Put in positions for block. Remember that coordinates in C style */ 
      pau->i = i;    
      pau->j = j;    
      if( isspecial != TRUE)     
        pau->state = state;  
      else pau->state = state + 2;   
      prev->score = cellscore;   
      prev = pau;    
      } /* end of while state != START */ 


    invert_PackAln(out); 
    return out;  
}    


/* Function:  find_end_TransRegionMatrix(mat,ri,rj,state,isspecial,h)
 *
 * Descrip: No Description
 *
 * Arg:              mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:               ri [UNKN ] Undocumented argument [int *]
 * Arg:               rj [UNKN ] Undocumented argument [int *]
 * Arg:            state [UNKN ] Undocumented argument [int *]
 * Arg:        isspecial [UNKN ] Undocumented argument [boolean *]
 * Arg:                h [UNKN ] Undocumented argument [TransRegionMatrix_access_func_holder]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int find_end_TransRegionMatrix(TransRegionMatrix * mat,int * ri,int * rj,int * state,boolean * isspecial,TransRegionMatrix_access_func_holder h) 
{
    int j;   
    int max; 
    int maxj;    
    int temp;    


    max = (*h.access_special)(mat,0,mat->seq->len-1,END);    
    maxj = mat->seq->len-1;  
    for(j= mat->seq->len-2 ;j >= 0 ;j--) {  
      if( (temp =(*h.access_special)(mat,0,j,END)) > max )   {  
        max = temp;  
        maxj = j;    
        }  
      }  


    if( ri != NULL)  
       *ri = 0;  
    if( rj != NULL)  
       *rj = maxj;   
    if( state != NULL)   
       *state = END; 
    if( isspecial != NULL)   
       *isspecial = TRUE;    


    return max;  
}    


/* Function:  TransRegionMatrix_debug_show_matrix(mat,starti,stopi,startj,stopj,ofp)
 *
 * Descrip: No Description
 *
 * Arg:           mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:        starti [UNKN ] Undocumented argument [int]
 * Arg:         stopi [UNKN ] Undocumented argument [int]
 * Arg:        startj [UNKN ] Undocumented argument [int]
 * Arg:         stopj [UNKN ] Undocumented argument [int]
 * Arg:           ofp [UNKN ] Undocumented argument [FILE *]
 *
 */
void TransRegionMatrix_debug_show_matrix(TransRegionMatrix * mat,int starti,int stopi,int startj,int stopj,FILE * ofp) 
{
    register int i;  
    register int j;  


    for(i=starti;i<stopi && i < mat->model->len;i++) {  
      for(j=startj;j<stopj && j < mat->seq->len;j++) {  
        fprintf(ofp,"Cell [%d - %d]\n",i,j);     
        fprintf(ofp,"State REGION %d\n",TransRegionMatrix_EXPL_MATRIX(mat,i,j,REGION));  
        fprintf(ofp,"State NON_REGION %d\n",TransRegionMatrix_EXPL_MATRIX(mat,i,j,NON_REGION));  
        fprintf(ofp,"\n\n"); 
        }  
      }  


}    


/* Function:  max_calc_TransRegionMatrix(mat,i,j,state,isspecial,reti,retj,retstate,retspecial,cellscore,h)
 *
 * Descrip: No Description
 *
 * Arg:               mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:                 i [UNKN ] Undocumented argument [int]
 * Arg:                 j [UNKN ] Undocumented argument [int]
 * Arg:             state [UNKN ] Undocumented argument [int]
 * Arg:         isspecial [UNKN ] Undocumented argument [boolean]
 * Arg:              reti [UNKN ] Undocumented argument [int *]
 * Arg:              retj [UNKN ] Undocumented argument [int *]
 * Arg:          retstate [UNKN ] Undocumented argument [int *]
 * Arg:        retspecial [UNKN ] Undocumented argument [boolean *]
 * Arg:         cellscore [UNKN ] Undocumented argument [int *]
 * Arg:                 h [UNKN ] Undocumented argument [TransRegionMatrix_access_func_holder]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int max_calc_TransRegionMatrix(TransRegionMatrix * mat,int i,int j,int state,boolean isspecial,int * reti,int * retj,int * retstate,boolean * retspecial,int * cellscore,TransRegionMatrix_access_func_holder h) 
{
    register int temp;   
    register int cscore; 


    *reti = (*retj) = (*retstate) = TransRegionMatrix_READ_OFF_ERROR;    


    if( i < 0 || j < 0 || i > mat->model->len || j > mat->seq->len)  {  
      warn("In TransRegionMatrix matrix special read off - out of bounds on matrix [i,j is %d,%d state %d in standard matrix]",i,j,state);   
      return -1;     
      }  


    /* Then you have to select the correct switch statement to figure out the readoff      */ 
    /* Somewhat odd - reverse the order of calculation and return as soon as it is correct */ 
    cscore = (*h.access_main)(mat,i,j,state);    
    switch(state)    { /*Switch state */ 
      case REGION :  
        temp = cscore - (mat->model->region_start) -  (0);   
        if( temp == (*h.access_main)(mat,i - 0,j - 1,NON_REGION) )   {  
          *reti = i - 0; 
          *retj = j - 1; 
          *retstate = NON_REGION;    
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - (*h.access_main)(mat,i-0,j-1,NON_REGION);  
            }  
          return (*h.access_main)(mat,i - 0,j - 1,NON_REGION);   
          }  
        temp = cscore - ((TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model))) -  (0); 
        if( temp == (*h.access_special)(mat,i - 0,j - 1,START) ) {  
          *reti = i - 0; 
          *retj = j - 1; 
          *retstate = START; 
          *retspecial = TRUE;    
          if( cellscore != NULL) {  
            *cellscore = cscore - (*h.access_special)(mat,i-0,j-1,START);    
            }  
          return (*h.access_main)(mat,i - 0,j - 1,START);    
          }  
        temp = cscore - ((TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model))) -  (0); 
        if( temp == (*h.access_main)(mat,i - 0,j - 1,REGION) )   {  
          *reti = i - 0; 
          *retj = j - 1; 
          *retstate = REGION;    
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - (*h.access_main)(mat,i-0,j-1,REGION);  
            }  
          return (*h.access_main)(mat,i - 0,j - 1,REGION);   
          }  
        warn("Major problem (!) - in TransRegionMatrix read off, position %d,%d state %d no source found!",i,j,state);   
        return (-1); 
      case NON_REGION :  
        temp = cscore - (0) -  (0);  
        if( temp == (*h.access_main)(mat,i - 0,j - 1,REGION) )   {  
          *reti = i - 0; 
          *retj = j - 1; 
          *retstate = REGION;    
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - (*h.access_main)(mat,i-0,j-1,REGION);  
            }  
          return (*h.access_main)(mat,i - 0,j - 1,REGION);   
          }  
        temp = cscore - (TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model)) -  (0);    
        if( temp == (*h.access_special)(mat,i - 0,j - 1,START) ) {  
          *reti = i - 0; 
          *retj = j - 1; 
          *retstate = START; 
          *retspecial = TRUE;    
          if( cellscore != NULL) {  
            *cellscore = cscore - (*h.access_special)(mat,i-0,j-1,START);    
            }  
          return (*h.access_main)(mat,i - 0,j - 1,START);    
          }  
        temp = cscore - (TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model)) -  (0);    
        if( temp == (*h.access_main)(mat,i - 0,j - 1,NON_REGION) )   {  
          *reti = i - 0; 
          *retj = j - 1; 
          *retstate = NON_REGION;    
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - (*h.access_main)(mat,i-0,j-1,NON_REGION);  
            }  
          return (*h.access_main)(mat,i - 0,j - 1,NON_REGION);   
          }  
        warn("Major problem (!) - in TransRegionMatrix read off, position %d,%d state %d no source found!",i,j,state);   
        return (-1); 
      default:   
        warn("Major problem (!) - in TransRegionMatrix read off, position %d,%d state %d no source found!",i,j,state);   
        return (-1); 
      } /* end of Switch state  */ 
}    


/* Function:  max_calc_special_TransRegionMatrix(mat,i,j,state,isspecial,reti,retj,retstate,retspecial,cellscore,h)
 *
 * Descrip: No Description
 *
 * Arg:               mat [UNKN ] Undocumented argument [TransRegionMatrix *]
 * Arg:                 i [UNKN ] Undocumented argument [int]
 * Arg:                 j [UNKN ] Undocumented argument [int]
 * Arg:             state [UNKN ] Undocumented argument [int]
 * Arg:         isspecial [UNKN ] Undocumented argument [boolean]
 * Arg:              reti [UNKN ] Undocumented argument [int *]
 * Arg:              retj [UNKN ] Undocumented argument [int *]
 * Arg:          retstate [UNKN ] Undocumented argument [int *]
 * Arg:        retspecial [UNKN ] Undocumented argument [boolean *]
 * Arg:         cellscore [UNKN ] Undocumented argument [int *]
 * Arg:                 h [UNKN ] Undocumented argument [TransRegionMatrix_access_func_holder]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int max_calc_special_TransRegionMatrix(TransRegionMatrix * mat,int i,int j,int state,boolean isspecial,int * reti,int * retj,int * retstate,boolean * retspecial,int * cellscore,TransRegionMatrix_access_func_holder h) 
{
    register int temp;   
    register int cscore; 


    *reti = (*retj) = (*retstate) = TransRegionMatrix_READ_OFF_ERROR;    


    if( j < 0 || j > mat->seq->len)  {  
      warn("In TransRegionMatrix matrix special read off - out of bounds on matrix [j is %d in special]",j); 
      return -1;     
      }  


    cscore = (*h.access_special)(mat,i,j,state); 
    switch(state)    { /*switch on special states*/ 
      case START :   
      case END :     
        /* source REGION is from main matrix */ 
        for(i= mat->model->len-1;i >= 0 ;i--)    { /*for i >= 0*/ 
          temp = cscore - (0) - (0);     
          if( temp == (*h.access_main)(mat,i - 0,j - 0,REGION) ) {  
            *reti = i - 0;   
            *retj = j - 0;   
            *retstate = REGION;  
            *retspecial = FALSE; 
            if( cellscore != NULL)   {  
              *cellscore = cscore - (*h.access_main)(mat,i-0,j-0,REGION);    
              }  
            return (*h.access_main)(mat,i - 0,j - 0,REGION) ;    
            }  
          } /* end of for i >= 0 */ 
        /* source NON_REGION is from main matrix */ 
        for(i= mat->model->len-1;i >= 0 ;i--)    { /*for i >= 0*/ 
          temp = cscore - (0) - (0);     
          if( temp == (*h.access_main)(mat,i - 0,j - 0,NON_REGION) ) {  
            *reti = i - 0;   
            *retj = j - 0;   
            *retstate = NON_REGION;  
            *retspecial = FALSE; 
            if( cellscore != NULL)   {  
              *cellscore = cscore - (*h.access_main)(mat,i-0,j-0,NON_REGION);    
              }  
            return (*h.access_main)(mat,i - 0,j - 0,NON_REGION) ;    
            }  
          } /* end of for i >= 0 */ 
      default:   
        warn("Major problem (!) - in TransRegionMatrix read off, position %d,%d state %d no source found  dropped into default on source switch!",i,j,state);    
        return (-1); 
      } /* end of switch on special states */ 
}    


/* Function:  calculate_TransRegionMatrix(mat)
 *
 * Descrip:    This function calculates the TransRegionMatrix matrix when in explicit mode
 *             To allocate the matrix use /allocate_Expl_TransRegionMatrix
 *
 *
 * Arg:        mat [UNKN ] TransRegionMatrix which contains explicit basematrix memory [TransRegionMatrix *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
boolean calculate_TransRegionMatrix(TransRegionMatrix * mat) 
{
    int i;   
    int j;   
    int leni;    
    int lenj;    
    int tot; 
    int num; 


    if( mat->basematrix->type != BASEMATRIX_TYPE_EXPLICIT )  {  
      warn("in calculate_TransRegionMatrix, passed a non Explicit matrix type, cannot calculate!");  
      return FALSE;  
      }  


    leni = mat->leni;    
    lenj = mat->lenj;    
    tot = leni * lenj;   
    num = 0; 


    start_reporting("TransRegionMatrix Matrix calculation: ");   
    for(j=0;j<lenj;j++)  {  
      auto int score;    
      auto int temp;     
      for(i=0;i<leni;i++)    {  
        if( num%1000 == 0 )  
          log_full_error(REPORT,0,"[%7d] Cells %2d%%%%",num,num*100/tot);    
        num++;   


        /* For state REGION */ 
        /* setting first movement to score */ 
        score = TransRegionMatrix_EXPL_MATRIX(mat,i-0,j-1,REGION) + (TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model));  
        /* From state START to state REGION */ 
        temp = TransRegionMatrix_EXPL_SPECIAL(mat,i-0,j-1,START) + (TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model));   
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state NON_REGION to state REGION */ 
        temp = TransRegionMatrix_EXPL_MATRIX(mat,i-0,j-1,NON_REGION) + mat->model->region_start;     
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for REGION */ 
        /* Add any movement independant score and put away */ 
         TransRegionMatrix_EXPL_MATRIX(mat,i,j,REGION) = score;  


        /* state REGION is a source for special END */ 
        temp = score + (0) + (0) ;   
        if( temp > TransRegionMatrix_EXPL_SPECIAL(mat,i,j,END) )     {  
          TransRegionMatrix_EXPL_SPECIAL(mat,i,j,END) = temp;    
          }  




        /* Finished calculating state REGION */ 


        /* For state NON_REGION */ 
        /* setting first movement to score */ 
        score = TransRegionMatrix_EXPL_MATRIX(mat,i-0,j-1,NON_REGION) + TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model);     
        /* From state START to state NON_REGION */ 
        temp = TransRegionMatrix_EXPL_SPECIAL(mat,i-0,j-1,START) + TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model);  
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state REGION to state NON_REGION */ 
        temp = TransRegionMatrix_EXPL_MATRIX(mat,i-0,j-1,REGION) + 0;    
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for NON_REGION */ 
        /* Add any movement independant score and put away */ 
         TransRegionMatrix_EXPL_MATRIX(mat,i,j,NON_REGION) = score;  


        /* state NON_REGION is a source for special END */ 
        temp = score + (0) + (0) ;   
        if( temp > TransRegionMatrix_EXPL_SPECIAL(mat,i,j,END) )     {  
          TransRegionMatrix_EXPL_SPECIAL(mat,i,j,END) = temp;    
          }  




        /* Finished calculating state NON_REGION */ 
        }  


      /* Special state START has no special to special movements */ 


      /* Special state END has no special to special movements */ 
      }  
    stop_reporting();    
    return TRUE;     
}    


/* Function:  calculate_dpenv_TransRegionMatrix(mat,dpenv)
 *
 * Descrip:    This function calculates the TransRegionMatrix matrix when in explicit mode, subject to the envelope
 *
 *
 * Arg:          mat [UNKN ] TransRegionMatrix which contains explicit basematrix memory [TransRegionMatrix *]
 * Arg:        dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
boolean calculate_dpenv_TransRegionMatrix(TransRegionMatrix * mat,DPEnvelope * dpenv) 
{
    int i;   
    int j;   
    int k;   
    int starti;  
    int startj;  
    int endi;    
    int endj;    
    int tot; 
    int num; 
    int should_calc; 


    if( mat->basematrix->type != BASEMATRIX_TYPE_EXPLICIT )  {  
      warn("in calculate_TransRegionMatrix, passed a non Explicit matrix type, cannot calculate!");  
      return FALSE;  
      }  


    prepare_DPEnvelope(dpenv);   
    starti = dpenv->starti;  
    if( starti < 0 ) 
      starti = 0;    
    startj = dpenv->startj;  
    if( startj < 0 ) 
      startj = 0;    
    endi = dpenv->endi;  
    if( endi > mat->leni )   
      endi = mat->leni;  
    endj = dpenv->endj;  
    if( endj > mat->lenj )   
      endj = mat->lenj;  
    tot = (endi-starti) * (endj-startj); 
    num = 0; 


    for(j=startj-1;j<endj;j++)   {  
      for(i=0;i<mat->leni;i++)   {  
        TransRegionMatrix_EXPL_MATRIX(mat,i,j,REGION) = NEGI;    
        TransRegionMatrix_EXPL_MATRIX(mat,i,j,NON_REGION) = NEGI;    
        }  
      }  
    for(j=-1;j<mat->lenj;j++)    {  
      TransRegionMatrix_EXPL_SPECIAL(mat,i,j,START) = 0; 
      TransRegionMatrix_EXPL_SPECIAL(mat,i,j,END) = NEGI;    
      }  


    start_reporting("TransRegionMatrix Matrix calculation: ");   
    for(j=startj;j<endj;j++) {  
      auto int score;    
      auto int temp;     
      for(i=starti;i<endi;i++)   {  
        /* Check if is in envelope - code identical to is_in_DPEnvelope, but aggressively inlined here for speed */ 
        should_calc = 0; 
        for(k=0;k<dpenv->len;k++)    {  
          auto DPUnit * u;   
          u = dpenv->dpu[k]; 
          switch(u->type)    {  
            case DPENV_RECT :    
              if( i >= u->starti && j >= u->startj && i <= (u->starti+u->height) && j <= (u->startj+u->length))  
                should_calc = 1;     
              break; 
            case DPENV_DIAG :    
              if(  abs( (i-j) - (u->starti-u->startj)) <= u->height && i+j >= u->starti+u->startj && i+j+u->length >= u->starti+u->startj)   
                should_calc = 1;     
              break; 
            }  
          if( should_calc == 1 ) 
            break;   
          }  
        if( should_calc == 0)    {  
          TransRegionMatrix_EXPL_MATRIX(mat,i,j,REGION) = NEGI;  
          TransRegionMatrix_EXPL_MATRIX(mat,i,j,NON_REGION) = NEGI;  
          continue;  
          }  


        if( num%1000 == 0 )  
          log_full_error(REPORT,0,"[%7d] Cells %2d%%%%",num,num*100/tot);    
        num++;   


        /* For state REGION */ 
        /* setting first movement to score */ 
        score = TransRegionMatrix_EXPL_MATRIX(mat,i-0,j-1,REGION) + (TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model));  
        /* From state START to state REGION */ 
        temp = TransRegionMatrix_EXPL_SPECIAL(mat,i-0,j-1,START) + (TRANS_REGION_MODEL_SCORE(mat->seq,j,mat->model)+TRANS_REGION_GC_SCORE(mat->seq,j,mat->model));   
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state NON_REGION to state REGION */ 
        temp = TransRegionMatrix_EXPL_MATRIX(mat,i-0,j-1,NON_REGION) + mat->model->region_start;     
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for REGION */ 
        /* Add any movement independant score and put away */ 
         TransRegionMatrix_EXPL_MATRIX(mat,i,j,REGION) = score;  


        /* state REGION is a source for special END */ 
        temp = score + (0) + (0) ;   
        if( temp > TransRegionMatrix_EXPL_SPECIAL(mat,i,j,END) )     {  
          TransRegionMatrix_EXPL_SPECIAL(mat,i,j,END) = temp;    
          }  




        /* Finished calculating state REGION */ 


        /* For state NON_REGION */ 
        /* setting first movement to score */ 
        score = TransRegionMatrix_EXPL_MATRIX(mat,i-0,j-1,NON_REGION) + TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model);     
        /* From state START to state NON_REGION */ 
        temp = TransRegionMatrix_EXPL_SPECIAL(mat,i-0,j-1,START) + TRANS_NON_REGION_MODEL_SCORE(mat->seq,j,mat->model);  
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state REGION to state NON_REGION */ 
        temp = TransRegionMatrix_EXPL_MATRIX(mat,i-0,j-1,REGION) + 0;    
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for NON_REGION */ 
        /* Add any movement independant score and put away */ 
         TransRegionMatrix_EXPL_MATRIX(mat,i,j,NON_REGION) = score;  


        /* state NON_REGION is a source for special END */ 
        temp = score + (0) + (0) ;   
        if( temp > TransRegionMatrix_EXPL_SPECIAL(mat,i,j,END) )     {  
          TransRegionMatrix_EXPL_SPECIAL(mat,i,j,END) = temp;    
          }  




        /* Finished calculating state NON_REGION */ 
        }  


      /* Special state START has no special to special movements */ 


      /* Special state END has no special to special movements */ 
      }  
    stop_reporting();    
    return TRUE;     
}    


/* Function:  TransRegionMatrix_alloc(void)
 *
 * Descrip:    Allocates structure: assigns defaults if given 
 *
 *
 *
 * Return [UNKN ]  Undocumented return value [TransRegionMatrix *]
 *
 */
TransRegionMatrix * TransRegionMatrix_alloc(void) 
{
    TransRegionMatrix * out;/* out is exported at end of function */ 


    /* call ckalloc and see if NULL */ 
    if((out=(TransRegionMatrix *) ckalloc (sizeof(TransRegionMatrix))) == NULL)  {  
      warn("TransRegionMatrix_alloc failed ");   
      return NULL;  /* calling function should respond! */ 
      }  
    out->dynamite_hard_link = 1; 
#ifdef PTHREAD   
    pthread_mutex_init(&(out->dynamite_mutex),NULL);     
#endif   
    out->basematrix = NULL;  
    out->shatter = NULL; 
    out->leni = 0;   
    out->lenj = 0;   


    return out;  
}    


/* Function:  free_TransRegionMatrix(obj)
 *
 * Descrip:    Free Function: removes the memory held by obj
 *             Will chain up to owned members and clear all lists
 *
 *
 * Arg:        obj [UNKN ] Object that is free'd [TransRegionMatrix *]
 *
 * Return [UNKN ]  Undocumented return value [TransRegionMatrix *]
 *
 */
TransRegionMatrix * free_TransRegionMatrix(TransRegionMatrix * obj) 
{
    int return_early = 0;    


    if( obj == NULL) {  
      warn("Attempting to free a NULL pointer to a TransRegionMatrix obj. Should be trappable"); 
      return NULL;   
      }  


#ifdef PTHREAD   
    assert(pthread_mutex_lock(&(obj->dynamite_mutex)) == 0); 
#endif   
    if( obj->dynamite_hard_link > 1)     {  
      return_early = 1;  
      obj->dynamite_hard_link--; 
      }  
#ifdef PTHREAD   
    assert(pthread_mutex_unlock(&(obj->dynamite_mutex)) == 0);   
#endif   
    if( return_early == 1)   
      return NULL;   
    if( obj->basematrix != NULL) 
      free_BaseMatrix(obj->basematrix);  
    if( obj->shatter != NULL)    
      free_ShatterMatrix(obj->shatter);  
    /* obj->model is linked in */ 
    /* obj->seq is linked in */ 


    ckfree(obj); 
    return NULL; 
}    





#ifdef _cplusplus
}
#endif
