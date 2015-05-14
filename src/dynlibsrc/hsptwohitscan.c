#ifdef _cplusplus
extern "C" {
#endif
#include "hsptwohitscan.h"

#include <sys/time.h>
#include <sys/resource.h>


/* Function:  new_twohit_one_off_HSPScanInterface(sli,mat,drop_off,score_cutoff)
 *
 * Descrip:    Builds a twohit scan interface. This
 *             does expands the query using a matrix but
 *             just be considering off by one cases
 *
 *
 * Arg:                 sli [UNKN ] Undocumented argument [SeqLookupInterface *]
 * Arg:                 mat [UNKN ] Undocumented argument [CompMat *]
 * Arg:            drop_off [UNKN ] Undocumented argument [int]
 * Arg:        score_cutoff [UNKN ] Undocumented argument [int]
 *
 * Return [UNKN ]  Undocumented return value [HSPScanInterface *]
 *
 */
# line 61 "hsptwohitscan.dy"
HSPScanInterface * new_twohit_one_off_HSPScanInterface(SeqLookupInterface * sli,CompMat * mat,int drop_off,int score_cutoff)
{

  HSPScanInterface * out;
  HSPScanPara * p;

  assert(sli);
  assert(mat);
  
  out = HSPScanInterface_alloc();

  p = HSPScanPara_alloc();
  p->sli = hard_link_SeqLookupInterface(sli);
  p->mat = hard_link_CompMat(mat);
  p->drop_off = drop_off;
  p->score_cutoff = score_cutoff;

  out->data = (void*)p;
  out->free_data = simple_HSPScan_free;
  if( sli->lookup_array_head == NULL ) {
    fatal("Currently can only use direct lookup in twohit scans");
  } else {
    out->scan_query = twohit_one_off_HSPscan_scan_query_direct;
  }

  return out;
}


/* Function:  twohit_one_off_HSPscan_scan_query_direct(data,seq,para)
 *
 * Descrip:    Word expansion with two hit semantics
 *
 *
 * Arg:        data [UNKN ] Undocumented argument [void *]
 * Arg:         seq [UNKN ] Undocumented argument [Sequence *]
 * Arg:        para [UNKN ] Undocumented argument [HSPScanInterfacePara *]
 *
 * Return [UNKN ]  Undocumented return value [LinearHSPmanager *]
 *
 */
# line 93 "hsptwohitscan.dy"
LinearHSPmanager * twohit_one_off_HSPscan_scan_query_direct(void * data,Sequence * seq,HSPScanInterfacePara * para)
{
  char * std_aa = "ACDEFGHIKLMNPQRSTVWY";
  HSPmanager * hspm;
  LinearHSPmanager * out;
  LineariseHSPPara * hsp_para;
  HSPScanPara * p = (HSPScanPara *)data;
  int i;
  int j;
  int k;
  int aa;
  int score;
  char newseq[5];

  TwoHitBuffer * thb;
  TwoHitStore  * s;

  int jj;
  int aa2;

  int has_seen;

  int seqno[5];
  int base[5];
  int start_base;

  int self_score[5];
  int total_score;
  int current_score;

  ArraySeqHead * head;
  int no;

#ifdef LINUX_TIMER
  struct timeval t1;
  struct timeval t2;
  struct timeval t3;
#endif

  assert(seq);
  assert(p);
  assert(para);
  assert(para->max_results > 0);


  if( VERBOSITY_CHECK(1,para->verbosity) ) {
    info("processing sequence %s with twohit implementation",seq->name);
  }


  hsp_para = LineariseHSPPara_alloc();
  hsp_para->verbosity   = para->verbosity;
  hsp_para->max_size    = para->max_results;
  hsp_para->min_score   = para->min_hsp_score;
  hsp_para->width       = para->hsp_link_width;
  hsp_para->tail        = para->hsp_link_length;
  

  for(i=0,start_base=1;i<5;i++) {
    base[i] = start_base;
    start_base = start_base * 26;
  }


  hspm = new_HSPmanager(seq,p->mat,p->drop_off);

  thb = new_TwoHitBuffer();

#ifdef LINUX_TIMER
  gettimeofday(&t1,NULL);
#endif

  /*
   * normal scan for 0 and 1 off. On identical and one off, simply put
   * things into the TwoHitBuffer and set the hit_state to TWOHIT_HANDLED - indicating
   * that this hit has already been delt with.
   *
   * For the two off cases - where most of the random cases will come
   * in, put in the TwoHitBuffer and only process the HSPs when you come
   * come back with a hit_state of TWOHIT_FIRST_ENTRY. This calling code
   * will then change this to TWOHIT_FIRST_STORED. If the hit_state is
   * TWOHIT_FIRST_STORED it will then process both the current hit and
   * the stored hit, and set the state to TWOHIT_HANDLED. If it is
   * in TWOHIT_HANDLED, it handles just the current hit.
   *
   * The end result of all of this is that a sequence will need two
   * two-off (or better) words to trigger HSP extension, which hopefully will
   * drastically cut down on random HSP extensions.
   *
   */


  for(i=0;i<seq->len-5;i++) {
    
    head = (*p->sli->lookup_array_head)(p->sli->data,seq_number_aa_5mer_client(seq->seq+i));
    if( head != NULL ) {

      if( para->numb_level < head->current_pos  || (ARRAYHEAD_IS_LOWCOMPLEXITY(head) && para->low_numb > 0 && para->low_numb <= head->current_pos) ) {
	if( VERBOSITY_CHECK(7,para->verbosity) ) {
	  info("position %d hit direct over filled position, %d vs hard %d, low %d",i,head->current_pos,para->numb_level,para->low_numb);
	}
	continue;
      }

      for(k=0;k<head->current_pos;k++) {
	s = add_to_TwoHitBuffer(thb,head->units[k].seq,i,head->units[k].pos);
	s->hit_state = TWOHIT_HANDLED;

	if( add_pair_HSPmanager_score(hspm,head->units[k].seq,i,head->units[k].pos,para->min_hsp_score) == TRUE ) {
	  ;
	}
      }
    }

    

    total_score = 0;
    for(score=0,j=0;j<5;j++) {

      seqno[j] = base[j]*(toupper(seq->seq[i+j])-'A');
      self_score[j] = p->mat->comp[toupper(seq->seq[i+j])-'A'][toupper(seq->seq[i+j])-'A'];
      total_score += self_score[j];
    }

    has_seen = 0;

    for(j=0;j<5;j++) {
      for(jj=1;jj<5;jj+=2) {
	for(aa=0;aa<20;aa++) {
	  for(aa2=0;aa2<20;aa2++) {

	    if( seq->seq[i+j] == std_aa[aa] ) {
	      continue;
	    }

	    current_score = total_score;

	    current_score -= (self_score[j] - p->mat->comp[toupper(seq->seq[i+j])-'A'][std_aa[aa]-'A']);
	    current_score -= (self_score[jj] - p->mat->comp[toupper(seq->seq[i+jj])-'A'][std_aa[aa2]-'A']);
	    
	    if( current_score < para->min_word_score ) {
	      continue;
	    } else {
	      /* fprintf(stderr,"Handling %d,%d amino acid %d,%d score %d\n",j,jj,aa,aa2,current_score); */
	    }
	    
	    seqno[j]  = base[j]*(std_aa[aa]-'A');
	    seqno[jj] = base[jj]*(std_aa[aa2]-'A');
	    
	    no= seqno[0]+seqno[1]+seqno[2]+seqno[3]+seqno[4];

	    head = (*p->sli->lookup_array_head)(p->sli->data,no);
	    if( head != NULL ) {
	      if( para->numb_level < head->current_pos  || (ARRAYHEAD_IS_LOWCOMPLEXITY(head) && para->low_numb > 0 && para->low_numb <= head->current_pos) ) {
		if( VERBOSITY_CHECK(7,para->verbosity) ) {
		  info("position %d hit direct over filled position, %d vs hard %d, low %d",i,head->current_pos,para->numb_level,para->low_numb);
		}
		
		continue;
	      }
	      
	      has_seen = 1;
	      for(k=0;k<head->current_pos;k++) {
		
		s = add_to_TwoHitBuffer(thb,head->units[k].seq,i,head->units[k].pos);
		/* if this is the first time we've used, this just store it */
		if( s->hit_state == TWOHIT_FIRST_ENTRY ) {
		  /*	      fprintf(stderr,"%d,%d  %s, %d,%d added as first entry\n",j,aa,head->units[k].seq->name,i,head->units[k].pos); */
		  s->hit_state = TWOHIT_FIRST_STORED;
		  continue;
		}
		/* if we get this back with TWOHIT_FIRST_STORED, 
		 * process the stored hit and then process the current
		 * afterwards */
		
		if( s->hit_state == TWOHIT_FIRST_STORED ) {
		  /* fprintf(stderr,"%d,%d sequence %s, second hit, %d,%d vs %d,%d\n",j,aa,s->target->name,s->query_pos,s->target_pos,i,head->units[k].pos); */
		  
		  
		  s->hit_state = TWOHIT_HANDLED;
		  /* stored hit */
		  add_pair_HSPmanager_score(hspm,s->target,s->query_pos,s->target_pos,para->min_hsp_score);
		}
		

		/* current hit - either stored or handled */
		add_pair_HSPmanager(hspm,head->units[k].seq,i,head->units[k].pos) ;
	      }
	    }

	    seqno[j]   = base[j]*(toupper(seq->seq[i+j]-'A'));
	    seqno[jj]  = base[jj]*(toupper(seq->seq[i+jj]-'A'));
	    
	    newseq[j] = seq->seq[i+j];
	    newseq[jj] = seq->seq[i+jj];
	    
	  }
	}
      }
    }
  }


  if( VERBOSITY_CHECK(4,para->verbosity) ) {

    for(i=0,jj=0,k=0;i<thb->len;i++) {
      for(j=0;j<thb->buffer[i]->len;j++ ) {
	if( thb->buffer[i]->word[j]->hit_state == TWOHIT_HANDLED ) {
	  jj++;
	}
	k++;
      }
    }
    
    info("Two hit status: %d sequences, %d words %d handled",thb->len,k,jj);
  }

#ifdef LINUX_TIMER
  if( VERBOSITY_CHECK(3,para->verbosity) ) {
    gettimeofday(&t2,NULL);
  }
#endif

  if( para->use_protein_heuristic == TRUE ) {
    out = new_LinearHSPmanager_simple_heuristic(hspm,hsp_para);
  } else {
    out = new_LinearHSPmanager_flat(hspm);
  }

#ifdef LINUX_TIMER
 
  if( VERBOSITY_CHECK(3,para->verbosity) ) {
    gettimeofday(&t3,NULL);
    
    info("Sort clock point: Scan %f : Sort %f",
	 t2.tv_sec - t1.tv_sec + ((t2.tv_usec - t1.tv_usec) * 1e-6),
	 t3.tv_sec - t2.tv_sec + ((t2.tv_usec - t2.tv_usec) * 1e-6)
	 );
    
  }
#endif


  free_HSPmanager(hspm);

  return out;
}

/* Function:  add_to_TwoHitBuffer(thb,target,query_pos,target_pos)
 *
 * Descrip:    adds a new potential hit to the TwoHitBuffer, 
 *             returning the HitStore datastructure updated
 *             if hit is 1 then this is first entry
 *             if hit is more than 1 second
 *
 *
 * Arg:               thb [UNKN ] Undocumented argument [TwoHitBuffer *]
 * Arg:            target [UNKN ] Undocumented argument [Sequence *]
 * Arg:         query_pos [UNKN ] Undocumented argument [int]
 * Arg:        target_pos [UNKN ] Undocumented argument [int]
 *
 * Return [UNKN ]  Undocumented return value [TwoHitStore *]
 *
 */
# line 347 "hsptwohitscan.dy"
TwoHitStore * add_to_TwoHitBuffer(TwoHitBuffer * thb,Sequence * target,int query_pos,int target_pos)
{
  TwoHitSequence * seq;
  TwoHitStore * s;
  int diag;
  
  int i;
  int j;

  diag = query_pos - target_pos;

  if( (seq = g_hash_table_lookup(thb->target_hash,(gpointer)target)) != NULL ) {

    /* unrolling the first 2 positions */

    if( seq->len ==2 || seq->len == 1) {
      if( seq->word[0]->diagonal == diag ) {
	return seq->word[0];
      }
    }
    if( seq->len == 2 ) {
      if( seq->word[1]->diagonal == diag ) {
	return seq->word[1];
      }
    }

    /* use the hash to test the other positions */

    if( seq->len > 2 && (s = g_hash_table_lookup(seq->diagonal_hash,&diag)) != NULL ) {
      return s;
    }



    /* no position on this diagonal - make a new store */
    s = new_TwoHitStore_from_Allocator(thb->thba);
    s->target  = target;
    s->hit_state = TWOHIT_FIRST_ENTRY;
    s->target_pos = target_pos;
    s->query_pos  = query_pos;
    s->diagonal   = diag;
    
    add_TwoHitSequence(seq,s);
    g_hash_table_insert(seq->diagonal_hash,&(s->diagonal),s);

    return s;
  }

  /* else make a new seq and a new store, add it, pointer the hash to it */
  
  seq = new_TwoHitSequence();

  add_TwoHitBuffer(thb,seq);
  g_hash_table_insert(thb->target_hash,(gpointer)target,seq);
  
  s = new_TwoHitStore_from_Allocator(thb->thba);

  s->target  = target;
  s->hit_state = TWOHIT_FIRST_ENTRY;
  s->target_pos = target_pos;
  s->query_pos  = query_pos;
  s->diagonal   = query_pos - target_pos;

  add_TwoHitSequence(seq,s);
  g_hash_table_insert(seq->diagonal_hash,&(s->diagonal),s);

  return s;
}
 

/* Function:  new_TwoHitBuffer(void)
 *
 * Descrip:    makes a new TwoHitBuffer ready for use
 *
 *
 *
 * Return [UNKN ]  Undocumented return value [TwoHitBuffer *]
 *
 */
# line 420 "hsptwohitscan.dy"
TwoHitBuffer * new_TwoHitBuffer(void)
{
  TwoHitBuffer * b;

  b = TwoHitBuffer_alloc_std();
  b->target_hash = g_hash_table_new(g_direct_hash,g_direct_equal);
  b->thba = new_TwoHitStoreBlockAllocator();

  return b;
}

/* Function:  new_TwoHitSequence(void)
 *
 * Descrip:    make a new TwoHitSequence ready for use
 *
 *
 *
 * Return [UNKN ]  Undocumented return value [TwoHitSequence *]
 *
 */
# line 434 "hsptwohitscan.dy"
TwoHitSequence * new_TwoHitSequence(void)
{
  TwoHitSequence * seq;

  seq = TwoHitSequence_alloc_std();
  seq->diagonal_hash = g_hash_table_new(g_int_hash,g_int_equal);
  
  return seq;
}


/* Function:  free_TwoHitSequence(t)
 *
 * Descrip:    Frees the TwoHitSequence
 *
 *
 * Arg:        t [UNKN ] Undocumented argument [TwoHitSequence *]
 *
 * Return [UNKN ]  Undocumented return value [TwoHitSequence *]
 *
 */
# line 449 "hsptwohitscan.dy"
TwoHitSequence * free_TwoHitSequence(TwoHitSequence * t)
{

  g_hash_table_destroy(t->diagonal_hash);


  /* block allocator handles store memory */

  ckfree(t->word);
  ckfree(t);

  return NULL;

}



/* Function:  free_TwoHitBuffer(t)
 *
 * Descrip:    Frees the TwoHitBuffer
 *
 *
 * Arg:        t [UNKN ] Undocumented argument [TwoHitBuffer *]
 *
 * Return [UNKN ]  Undocumented return value [TwoHitBuffer *]
 *
 */
# line 470 "hsptwohitscan.dy"
TwoHitBuffer * free_TwoHitBuffer(TwoHitBuffer * t)
{
  int i;

  g_hash_table_destroy(t->target_hash);

  for(i=0;i<t->len;i++) {
    free_TwoHitSequence(t->buffer[i]);
  }
  ckfree(t->buffer);
  ckfree(t);

  return NULL;

}

/* Function:  TwoHitStore_alloc(void)
 *
 * Descrip:    allocator for twohitstore
 *
 *
 *
 * Return [UNKN ]  Undocumented return value [TwoHitStore *]
 *
 */
# line 489 "hsptwohitscan.dy"
TwoHitStore * TwoHitStore_alloc(void)
{
  TwoHitStore * out;

  out = (TwoHitStore *)malloc(sizeof(TwoHitStore));

  return out;
}

/* Function:  free_TwoHitStore(t)
 *
 * Descrip:    free for twohitstore
 *
 *
 * Arg:        t [UNKN ] Undocumented argument [TwoHitStore *]
 *
 * Return [UNKN ]  Undocumented return value [TwoHitStore *]
 *
 */
# line 501 "hsptwohitscan.dy"
TwoHitStore * free_TwoHitStore(TwoHitStore * t)
{
  if( t == NULL ) {
    return NULL;
  }
  free(t);
  return NULL;
}

/* Function:  new_TwoHitStore_from_Allocator(a)
 *
 * Descrip:    gets a new store from a block allocator
 *
 *
 * Arg:        a [UNKN ] Undocumented argument [TwoHitStoreBlockAllocator *]
 *
 * Return [UNKN ]  Undocumented return value [TwoHitStore *]
 *
 */
# line 513 "hsptwohitscan.dy"
TwoHitStore * new_TwoHitStore_from_Allocator(TwoHitStoreBlockAllocator * a)
{
  /* do short path first! */

  if( a->current_pos < TWOHIT_BLOCK_SIZE ) {
    return &(a->block[a->current_block][a->current_pos++]);
  }

  /* else, we need a new block */
  if( a->current_block >= a->block_len ) {
    /* we need to realloc the block ! */
    a->block = realloc(a->block,sizeof(TwoHitStore *)*(2*a->block_len));
    assert(a->block != NULL);
    a->block_len = (2*a->block_len);
  }

  a->current_block++;

  a->block[a->current_block] = calloc(TWOHIT_BLOCK_SIZE,sizeof(TwoHitStore));

  assert(a->block[a->current_block] != NULL);

  a->current_pos = 0;

  return &(a->block[a->current_block][a->current_pos++]);
}

/* Function:  free_TwoHitStoreBlockAllocator(a)
 *
 * Descrip:    frees a block allocator
 *
 *
 * Arg:        a [UNKN ] Undocumented argument [TwoHitStoreBlockAllocator *]
 *
 * Return [UNKN ]  Undocumented return value [TwoHitStoreBlockAllocator *]
 *
 */
# line 543 "hsptwohitscan.dy"
TwoHitStoreBlockAllocator * free_TwoHitStoreBlockAllocator(TwoHitStoreBlockAllocator * a)
{
  int i;

  assert(a != NULL);

  for(i=0;i<a->current_block;i++) {
    free(a->block[i]);
  }

  free(a->block);

  free(a);

  return NULL;
}


/* Function:  new_TwoHitStoreBlockAllocator(void)
 *
 * Descrip:    makes a new block allocator
 *
 *
 *
 * Return [UNKN ]  Undocumented return value [TwoHitStoreBlockAllocator *]
 *
 */
# line 564 "hsptwohitscan.dy"
TwoHitStoreBlockAllocator * new_TwoHitStoreBlockAllocator(void)
{
  TwoHitStoreBlockAllocator * out;

  out = (TwoHitStoreBlockAllocator*) malloc(sizeof(TwoHitStoreBlockAllocator));

  assert(out != NULL);

  out->block = (TwoHitStore **) calloc(TWOHIT_BLOCK_DEPTH,sizeof(TwoHitStore*));
  out->block_len = TWOHIT_BLOCK_DEPTH;
  out->block[0] = (TwoHitStore*) calloc(TWOHIT_BLOCK_SIZE,sizeof(TwoHitStore));
  
  out->current_block = 0;
  out->current_pos = 0;

  return out;

}


# line 611 "hsptwohitscan.c"
/* Function:  swap_TwoHitSequence(list,i,j)
 *
 * Descrip:    swap function: an internal for qsort_TwoHitSequence
 *             swaps two positions in the array
 *
 *
 * Arg:        list [UNKN ] List of structures to swap in [TwoHitStore **]
 * Arg:           i [UNKN ] swap position [int]
 * Arg:           j [UNKN ] swap position [int]
 *
 */
/* swap function for qsort function */ 
void swap_TwoHitSequence(TwoHitStore ** list,int i,int j)  
{
    TwoHitStore * temp;  
    temp=list[i];    
    list[i]=list[j]; 
    list[j]=temp;    
}    


/* Function:  qsort_TwoHitSequence(list,left,right,comp)
 *
 * Descrip:    qsort - lifted from K&R 
 *             sorts the array using quicksort
 *             Probably much better to call sort_TwoHitSequence which sorts from start to end
 *
 *
 * Arg:         list [UNKN ] List of structures to swap in [TwoHitStore **]
 * Arg:         left [UNKN ] left position [int]
 * Arg:        right [UNKN ] right position [int]
 * Arg:         comp [FUNCP] Function which returns -1 or 1 to sort on [int (*comp]
 *
 */
void qsort_TwoHitSequence(TwoHitStore ** list,int left,int right,int (*comp)(TwoHitStore * ,TwoHitStore * )) 
{
    int i,last;  
    if( left >= right )  
      return;    


    swap_TwoHitSequence(list,left,(left+right)/2);   
    last = left; 
    for ( i=left+1; i <= right;i++)  {  
      if( (*comp)(list[i],list[left]) < 0)   
        swap_TwoHitSequence (list,++last,i); 
      }  
    swap_TwoHitSequence (list,left,last);    
    qsort_TwoHitSequence(list,left,last-1,comp); 
    qsort_TwoHitSequence(list,last+1,right,comp);    
}    


/* Function:  sort_TwoHitSequence(obj,comp)
 *
 * Descrip:    sorts from start to end using comp 
 *             sorts the array using quicksort by calling qsort_TwoHitSequence
 *
 *
 * Arg:         obj [UNKN ] Object containing list [TwoHitSequence *]
 * Arg:        comp [FUNCP] Function which returns -1 or 1 to sort on [int (*comp]
 *
 */
void sort_TwoHitSequence(TwoHitSequence * obj,int (*comp)(TwoHitStore *, TwoHitStore *)) 
{
    qsort_TwoHitSequence(obj->word,0,obj->len-1,comp);   
    return;  
}    


/* Function:  expand_TwoHitSequence(obj,len)
 *
 * Descrip:    Really an internal function for add_TwoHitSequence
 *
 *
 * Arg:        obj [UNKN ] Object which contains the list [TwoHitSequence *]
 * Arg:        len [UNKN ] Length to add one [int]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
boolean expand_TwoHitSequence(TwoHitSequence * obj,int len) 
{


    if( obj->maxlen > obj->len )     {  
      warn("expand_TwoHitSequence called with no need"); 
      return TRUE;   
      }  


    if( (obj->word = (TwoHitStore ** ) ckrealloc (obj->word,sizeof(TwoHitStore *)*len)) == NULL)     {  
      warn("ckrealloc failed for expand_TwoHitSequence, returning FALSE");   
      return FALSE;  
      }  
    obj->maxlen = len;   
    return TRUE; 
}    


/* Function:  add_TwoHitSequence(obj,add)
 *
 * Descrip:    Adds another object to the list. It will expand the list if necessary
 *
 *
 * Arg:        obj [UNKN ] Object which contains the list [TwoHitSequence *]
 * Arg:        add [OWNER] Object to add to the list [TwoHitStore *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
/* will expand function if necessary */ 
boolean add_TwoHitSequence(TwoHitSequence * obj,TwoHitStore * add) 
{
    if( obj->len >= obj->maxlen) {  
      if( expand_TwoHitSequence(obj,obj->len + TwoHitSequenceLISTLENGTH) == FALSE)   
        return FALSE;    
      }  


    obj->word[obj->len++]=add;   
    return TRUE; 
}    


/* Function:  flush_TwoHitSequence(obj)
 *
 * Descrip:    Frees the list elements, sets length to 0
 *             If you want to save some elements, use hard_link_xxx
 *             to protect them from being actually destroyed in the free
 *
 *
 * Arg:        obj [UNKN ] Object which contains the list  [TwoHitSequence *]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int flush_TwoHitSequence(TwoHitSequence * obj) 
{
    int i;   


    for(i=0;i<obj->len;i++)  { /*for i over list length*/ 
      if( obj->word[i] != NULL)  {  
        free_TwoHitStore(obj->word[i]);  
        obj->word[i] = NULL; 
        }  
      } /* end of for i over list length */ 


    obj->len = 0;    
    return i;    
}    


/* Function:  TwoHitSequence_alloc_std(void)
 *
 * Descrip:    Equivalent to TwoHitSequence_alloc_len(TwoHitSequenceLISTLENGTH)
 *
 *
 *
 * Return [UNKN ]  Undocumented return value [TwoHitSequence *]
 *
 */
TwoHitSequence * TwoHitSequence_alloc_std(void) 
{
    return TwoHitSequence_alloc_len(TwoHitSequenceLISTLENGTH);   
}    


/* Function:  TwoHitSequence_alloc_len(len)
 *
 * Descrip:    Allocates len length to all lists
 *
 *
 * Arg:        len [UNKN ] Length of lists to allocate [int]
 *
 * Return [UNKN ]  Undocumented return value [TwoHitSequence *]
 *
 */
TwoHitSequence * TwoHitSequence_alloc_len(int len) 
{
    TwoHitSequence * out;   /* out is exported at the end of function */ 


    /* Call alloc function: return NULL if NULL */ 
    /* Warning message alread in alloc function */ 
    if((out = TwoHitSequence_alloc()) == NULL)   
      return NULL;   


    /* Calling ckcalloc for list elements */ 
    if((out->word = (TwoHitStore ** ) ckcalloc (len,sizeof(TwoHitStore *))) == NULL) {  
      warn("Warning, ckcalloc failed in TwoHitSequence_alloc_len");  
      return NULL;   
      }  
    out->len = 0;    
    out->maxlen = len;   


    return out;  
}    


/* Function:  hard_link_TwoHitSequence(obj)
 *
 * Descrip:    Bumps up the reference count of the object
 *             Meaning that multiple pointers can 'own' it
 *
 *
 * Arg:        obj [UNKN ] Object to be hard linked [TwoHitSequence *]
 *
 * Return [UNKN ]  Undocumented return value [TwoHitSequence *]
 *
 */
TwoHitSequence * hard_link_TwoHitSequence(TwoHitSequence * obj) 
{
    if( obj == NULL )    {  
      warn("Trying to hard link to a TwoHitSequence object: passed a NULL object");  
      return NULL;   
      }  
    obj->dynamite_hard_link++;   
    return obj;  
}    


/* Function:  TwoHitSequence_alloc(void)
 *
 * Descrip:    Allocates structure: assigns defaults if given 
 *
 *
 *
 * Return [UNKN ]  Undocumented return value [TwoHitSequence *]
 *
 */
TwoHitSequence * TwoHitSequence_alloc(void) 
{
    TwoHitSequence * out;   /* out is exported at end of function */ 


    /* call ckalloc and see if NULL */ 
    if((out=(TwoHitSequence *) ckalloc (sizeof(TwoHitSequence))) == NULL)    {  
      warn("TwoHitSequence_alloc failed ");  
      return NULL;  /* calling function should respond! */ 
      }  
    out->dynamite_hard_link = 1; 
#ifdef PTHREAD   
    pthread_mutex_init(&(out->dynamite_mutex),NULL);     
#endif   
    out->word = NULL;    
    out->len = out->maxlen = 0;  


    return out;  
}    


/* Function:  swap_TwoHitBuffer(list,i,j)
 *
 * Descrip:    swap function: an internal for qsort_TwoHitBuffer
 *             swaps two positions in the array
 *
 *
 * Arg:        list [UNKN ] List of structures to swap in [TwoHitSequence **]
 * Arg:           i [UNKN ] swap position [int]
 * Arg:           j [UNKN ] swap position [int]
 *
 */
/* swap function for qsort function */ 
void swap_TwoHitBuffer(TwoHitSequence ** list,int i,int j)  
{
    TwoHitSequence * temp;   
    temp=list[i];    
    list[i]=list[j]; 
    list[j]=temp;    
}    


/* Function:  qsort_TwoHitBuffer(list,left,right,comp)
 *
 * Descrip:    qsort - lifted from K&R 
 *             sorts the array using quicksort
 *             Probably much better to call sort_TwoHitBuffer which sorts from start to end
 *
 *
 * Arg:         list [UNKN ] List of structures to swap in [TwoHitSequence **]
 * Arg:         left [UNKN ] left position [int]
 * Arg:        right [UNKN ] right position [int]
 * Arg:         comp [FUNCP] Function which returns -1 or 1 to sort on [int (*comp]
 *
 */
void qsort_TwoHitBuffer(TwoHitSequence ** list,int left,int right,int (*comp)(TwoHitSequence * ,TwoHitSequence * )) 
{
    int i,last;  
    if( left >= right )  
      return;    


    swap_TwoHitBuffer(list,left,(left+right)/2); 
    last = left; 
    for ( i=left+1; i <= right;i++)  {  
      if( (*comp)(list[i],list[left]) < 0)   
        swap_TwoHitBuffer (list,++last,i);   
      }  
    swap_TwoHitBuffer (list,left,last);  
    qsort_TwoHitBuffer(list,left,last-1,comp);   
    qsort_TwoHitBuffer(list,last+1,right,comp);  
}    


/* Function:  sort_TwoHitBuffer(obj,comp)
 *
 * Descrip:    sorts from start to end using comp 
 *             sorts the array using quicksort by calling qsort_TwoHitBuffer
 *
 *
 * Arg:         obj [UNKN ] Object containing list [TwoHitBuffer *]
 * Arg:        comp [FUNCP] Function which returns -1 or 1 to sort on [int (*comp]
 *
 */
void sort_TwoHitBuffer(TwoHitBuffer * obj,int (*comp)(TwoHitSequence *, TwoHitSequence *)) 
{
    qsort_TwoHitBuffer(obj->buffer,0,obj->len-1,comp);   
    return;  
}    


/* Function:  expand_TwoHitBuffer(obj,len)
 *
 * Descrip:    Really an internal function for add_TwoHitBuffer
 *
 *
 * Arg:        obj [UNKN ] Object which contains the list [TwoHitBuffer *]
 * Arg:        len [UNKN ] Length to add one [int]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
boolean expand_TwoHitBuffer(TwoHitBuffer * obj,int len) 
{


    if( obj->maxlen > obj->len )     {  
      warn("expand_TwoHitBuffer called with no need");   
      return TRUE;   
      }  


    if( (obj->buffer = (TwoHitSequence ** ) ckrealloc (obj->buffer,sizeof(TwoHitSequence *)*len)) == NULL)   {  
      warn("ckrealloc failed for expand_TwoHitBuffer, returning FALSE"); 
      return FALSE;  
      }  
    obj->maxlen = len;   
    return TRUE; 
}    


/* Function:  add_TwoHitBuffer(obj,add)
 *
 * Descrip:    Adds another object to the list. It will expand the list if necessary
 *
 *
 * Arg:        obj [UNKN ] Object which contains the list [TwoHitBuffer *]
 * Arg:        add [OWNER] Object to add to the list [TwoHitSequence *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
/* will expand function if necessary */ 
boolean add_TwoHitBuffer(TwoHitBuffer * obj,TwoHitSequence * add) 
{
    if( obj->len >= obj->maxlen) {  
      if( expand_TwoHitBuffer(obj,obj->len + TwoHitBufferLISTLENGTH) == FALSE)   
        return FALSE;    
      }  


    obj->buffer[obj->len++]=add; 
    return TRUE; 
}    


/* Function:  flush_TwoHitBuffer(obj)
 *
 * Descrip:    Frees the list elements, sets length to 0
 *             If you want to save some elements, use hard_link_xxx
 *             to protect them from being actually destroyed in the free
 *
 *
 * Arg:        obj [UNKN ] Object which contains the list  [TwoHitBuffer *]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int flush_TwoHitBuffer(TwoHitBuffer * obj) 
{
    int i;   


    for(i=0;i<obj->len;i++)  { /*for i over list length*/ 
      if( obj->buffer[i] != NULL)    {  
        free_TwoHitSequence(obj->buffer[i]); 
        obj->buffer[i] = NULL;   
        }  
      } /* end of for i over list length */ 


    obj->len = 0;    
    return i;    
}    


/* Function:  TwoHitBuffer_alloc_std(void)
 *
 * Descrip:    Equivalent to TwoHitBuffer_alloc_len(TwoHitBufferLISTLENGTH)
 *
 *
 *
 * Return [UNKN ]  Undocumented return value [TwoHitBuffer *]
 *
 */
TwoHitBuffer * TwoHitBuffer_alloc_std(void) 
{
    return TwoHitBuffer_alloc_len(TwoHitBufferLISTLENGTH);   
}    


/* Function:  TwoHitBuffer_alloc_len(len)
 *
 * Descrip:    Allocates len length to all lists
 *
 *
 * Arg:        len [UNKN ] Length of lists to allocate [int]
 *
 * Return [UNKN ]  Undocumented return value [TwoHitBuffer *]
 *
 */
TwoHitBuffer * TwoHitBuffer_alloc_len(int len) 
{
    TwoHitBuffer * out; /* out is exported at the end of function */ 


    /* Call alloc function: return NULL if NULL */ 
    /* Warning message alread in alloc function */ 
    if((out = TwoHitBuffer_alloc()) == NULL) 
      return NULL;   


    /* Calling ckcalloc for list elements */ 
    if((out->buffer = (TwoHitSequence ** ) ckcalloc (len,sizeof(TwoHitSequence *))) == NULL) {  
      warn("Warning, ckcalloc failed in TwoHitBuffer_alloc_len");    
      return NULL;   
      }  
    out->len = 0;    
    out->maxlen = len;   


    return out;  
}    


/* Function:  hard_link_TwoHitBuffer(obj)
 *
 * Descrip:    Bumps up the reference count of the object
 *             Meaning that multiple pointers can 'own' it
 *
 *
 * Arg:        obj [UNKN ] Object to be hard linked [TwoHitBuffer *]
 *
 * Return [UNKN ]  Undocumented return value [TwoHitBuffer *]
 *
 */
TwoHitBuffer * hard_link_TwoHitBuffer(TwoHitBuffer * obj) 
{
    if( obj == NULL )    {  
      warn("Trying to hard link to a TwoHitBuffer object: passed a NULL object");    
      return NULL;   
      }  
    obj->dynamite_hard_link++;   
    return obj;  
}    


/* Function:  TwoHitBuffer_alloc(void)
 *
 * Descrip:    Allocates structure: assigns defaults if given 
 *
 *
 *
 * Return [UNKN ]  Undocumented return value [TwoHitBuffer *]
 *
 */
TwoHitBuffer * TwoHitBuffer_alloc(void) 
{
    TwoHitBuffer * out; /* out is exported at end of function */ 


    /* call ckalloc and see if NULL */ 
    if((out=(TwoHitBuffer *) ckalloc (sizeof(TwoHitBuffer))) == NULL)    {  
      warn("TwoHitBuffer_alloc failed ");    
      return NULL;  /* calling function should respond! */ 
      }  
    out->dynamite_hard_link = 1; 
#ifdef PTHREAD   
    pthread_mutex_init(&(out->dynamite_mutex),NULL);     
#endif   
    out->buffer = NULL;  
    out->len = out->maxlen = 0;  
    out->thba = NULL;    


    return out;  
}    



#ifdef _cplusplus
}
#endif
