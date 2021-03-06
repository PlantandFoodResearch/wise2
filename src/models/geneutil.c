#ifdef _cplusplus
extern "C" {
#endif
#include "geneutil.h"


/* Function:  new_GenomicRegion_from_GeneWise(gen,pseudo,alb)
 *
 * Descrip:    Makes a new GenomicRegion with the genes
 *             predicted from this AlnBlock
 *
 *             Really a wrapper around the add_Genes_to_GenomicRegion_GeneWise
 *             and other functionality
 *
 *
 * Arg:           gen [UNKN ] genomic sequence to use [Genomic *]
 * Arg:        pseudo [UNKN ] If true, predicts frameshifted genes as pseudogenes [boolean]
 * Arg:           alb [UNKN ] genewise alignment to predict genes from [AlnBlock *]
 *
 * Return [UNKN ]  a newly allocated structure [GenomicRegion *]
 *
 */
# line 25 "geneutil.dy"
GenomicRegion * new_GenomicRegion_from_GeneWise(Genomic * gen,boolean pseudo,AlnBlock * alb)
{
  GenomicRegion * out;

  out = new_GenomicRegion(gen);
  
  add_Genes_to_GenomicRegion_new(out,alb);

  return out;
}

/* Function:  add_Genes_to_GenomicRegion_GeneWise(gr,org_start,org_end,alb,root,pseudo,make_name)
 *
 * Descrip:    Potential an Alnblock may have more
 *             than one gene due to looping models
 *
 *             This adds all the genes to gr
 *
 *
 *
 * Arg:               gr [UNKN ] genomic region to add genes to [GenomicRegion *]
 * Arg:        org_start [UNKN ] start point of the dna to which the alb was made from [int]
 * Arg:          org_end [UNKN ] end point of the dna to which the alb was made from [int]
 * Arg:              alb [UNKN ] logical label alignment [AlnBlock *]
 * Arg:             root [UNKN ] the second argument to make_name [char *]
 * Arg:           pseudo [UNKN ] If true, frameshifted genes are predicted as pseudo genes [boolean]
 * Arg:        make_name [FUNCP] a pointer to a function to actually make the name of the gene [char * (*make_name]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
# line 51 "geneutil.dy"
int add_Genes_to_GenomicRegion_GeneWise(GenomicRegion * gr,int org_start,int org_end,AlnBlock * alb,char * root,boolean pseudo,char * (*make_name)(Wise2_Genomic *,char *,int ,Wise2_Gene * ))
{
  int count = 0;
  Gene * gene;
  AlnColumn * alc;

  alc = alb->start;

  while( (gene = Gene_from_AlnColumn_GeneWise(alc,org_start,org_end,pseudo,&alc)) != NULL ) {
    if( make_name != NULL ) {
      gene->name = (*make_name)(gr->genomic,root,gr->len,gene);
    } 
    if( root != NULL ) {
      gene->seqname = stringalloc(root);
    }

    add_Gene_to_GenomicRegion(gr,gene);
    count++;
    if( alc == NULL ) 
      break;
  }
  
  return count;
}


/* Function:  add_Genes_to_GenomicRegion_new(gr,alb)
 *
 * Descrip:    Adds genes using the new gene function
 *
 *
 * Arg:         gr [UNKN ] Undocumented argument [GenomicRegion *]
 * Arg:        alb [UNKN ] Undocumented argument [AlnBlock *]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
# line 80 "geneutil.dy"
int add_Genes_to_GenomicRegion_new(GenomicRegion * gr,AlnBlock * alb)
{
  int count = 0;
  Gene * out;
  Transcript * tr;
  Transcript * new_tr;
  Translation * ts;
  Exon * exon;

  AlnBlock * coll;
  AlnColumn * alc;
  AlnColumn * exon_start;
  AlnColumn * intron_start;
  
  int trace = 1;
  int exon_start_coord;
  int is_reversed = 0;
  int temp;
  int i;

  coll = collapsed_AlnBlock(alb,1);



  /* find start */
  for(alc = coll->start;alc != NULL;alc = alc->next ) {
    if( strstr(alc->alu[1]->text_label,"CODON") != NULL ) {

      if( trace ) 
	fprintf(stderr,"Got %s as first codon\n",alc->alu[1]->text_label);

      /* got a gene */
      out = Gene_alloc_len(1);
      add_Gene_to_GenomicRegion(gr,out);

      tr = Transcript_alloc_std();
      /* don't add transcript yet, because reversing it requires arsing around with exons */

      if( strstr(alc->alu[1]->text_label,"REV") != NULL ) {
	is_reversed = 1;
      } else {
	is_reversed = 0;
      }
      exon_start_coord = alc->alu[1]->start+1;
      out->start = exon_start_coord;
      /* first exon is different as there is no 3' splice site */

      exon_start = alc;
      alc = alc->next;
      if( alc == NULL ) {
	warn("weird death in gene parsing");
	return 1;
      }

      /* handle single exon genes */
      if( strstr(alc->alu[1]->text_label,"SS") == NULL ) {
	/* single exon gene */

	exon = Exon_alloc_std();
	exon->start = 0;
	exon->end   = alc->alu[1]->start+3 - exon_start->alu[1]->start;
	if( trace ) 
	  fprintf(stderr,"Single exon gene %s from %d to %d\n",alc->alu[1]->text_label,exon->start,exon->end);

	if( is_reversed == 0 ) {
	  out->start   = exon_start->alu[1]->start+1;
	  out->end     = alc->alu[1]->start+4;
	} else {
	  out->end     = exon_start->alu[1]->start;
	  out->start   = alc->alu[1]->start+3;
	}

	add_ex_Transcript(tr,exon);
	ts = Translation_alloc();
	ts->start = 0;
	ts->end = length_Transcript(tr);
	ts->parent = tr;
	add_Transcript(tr,ts);
	tr->parent = out;
	add_Gene(out,tr);
	
	continue;
      }

      /* handle first exon */
      exon = Exon_alloc_std();
      add_ex_Transcript(tr,exon);
      exon->start = exon_start->alu[1]->start+1 - exon_start_coord;

      if( strstr(alc->alu[1]->text_label,"PHASE_0") != NULL) {
	exon->end   = alc->alu[1]->start +1 - out->start;
      } else if (  strstr(alc->alu[1]->text_label,"PHASE_1") != NULL)  {
	exon->end   = alc->alu[1]->start +2 - out->start;
      } else if (  strstr(alc->alu[1]->text_label,"PHASE_2") != NULL) {
	exon->end   = alc->alu[1]->start +3 - out->start;
      }

      
      /* loop over internal exons. Alc is current on leftmost splice site */
      for( ;; ) {
	if( trace ) 
	  fprintf(stderr,"internal exon - this should be leftmost splice %s\n",alc->alu[1]->text_label);

	if( strstr(alc->next->alu[1]->text_label,"SS") != NULL ) {
	  warn("Have a no-base intron. Conceptually possible, but highly unlikely. Probably misparamterisation somewhere...");
	} else {
	  alc = alc->next;
	  if( trace ) 
	    fprintf(stderr,"internal exon - this should be INTRON %s\n",alc->alu[1]->text_label);
	}


	if( alc == NULL ) {
	  warn("Weird death inside Intron of gene parsing");
	  return 1;
	}
	alc = alc->next;

	if( trace ) 
	  fprintf(stderr,"internal exon - this should be rightmost splice %s\n",alc->alu[1]->text_label);


	if( alc == NULL ) {
	  warn("Weird death inside Intron of gene parsing");
	  return 1;
	}
	
	if( strstr(alc->alu[1]->text_label,"SS") == NULL ) {
	  warn("At end of intron , non splice label %s",alc->alu[1]->text_label);
	  return 1;
	}

	exon_start = alc;

	alc = alc->next;

	if( trace ) 
	  fprintf(stderr,"internal exon - this should be CODON %s\n",alc->alu[1]->text_label);

	if( alc == NULL ) {
	  warn("Weird death inside Intron of gene parsing");
	  return 1;
	}
	alc = alc->next;
	if( alc == NULL ) {
	  warn("Weird death inside Intron of gene parsing");
	  return 1;
	}

	if( trace ) 
	  fprintf(stderr,"internal exon - this should be 5SS %s\n",alc->alu[1]->text_label);

	/* should have a 3'SS whatever */
	exon = Exon_alloc_std();
	add_ex_Transcript(tr,exon);

	if( strstr(exon_start->alu[1]->text_label,"PHASE_0") != NULL) {
	  exon->start   = exon_start->alu[1]->start +4 - out->start;
	} else if (  strstr(exon_start->alu[1]->text_label,"PHASE_1") == NULL)  {
	  exon->start   = exon_start->alu[1]->start +4 - out->start;
	} else if (  strstr(exon_start->alu[1]->text_label,"PHASE_2") == NULL) {
	  exon->start   = exon_start->alu[1]->start +4 - out->start;
	}
	

	if( strstr(alc->alu[1]->text_label,"SS") == NULL ) {
	  fprintf(stderr,"Breaking with no 5'SS on %s with %d\n",alc->alu[1]->text_label,alc->alu[1]->start);
	  exon->end = alc->alu[1]->start +1   - out->start;
	  break;
	}  else {
	  if( strstr(alc->alu[1]->text_label,"PHASE_0") != NULL) {
	    exon->end   = alc->alu[1]->start +1 - out->start;
	  } else if (  strstr(alc->alu[1]->text_label,"PHASE_1") != NULL)  {
	    exon->end   = alc->alu[1]->start +2 - out->start;
	  } else if (  strstr(alc->alu[1]->text_label,"PHASE_2") != NULL) {
	    exon->end   = alc->alu[1]->start +3 - out->start;
	  }

	}
	/* alc is left on the 5'SS for the next exon */
      } /* end of for (;;) over internal exons*/

      /* end of a gene. exon is the last exon */
      out->end = exon->end + out->start;

      ts = Translation_alloc();
      ts->start = 0;
      ts->end = length_Transcript(tr);
      if( ts->end % 3 != 0 ) {
	warn("Transcript is not mod 3 size. It is %d doh!",ts->end);
      }

      if( is_reversed == 1 ) {
	if( trace ) 
	  fprintf(stderr,"Reversing gene %d to %d\n",out->start,out->end);
	temp = out->end;
	temp = temp;
	out->end = out->start;
	out->start = temp;
	
	/* reversed genes have an off by one convention */
	out->end -= 1;
	out->start -= 1;

	/* now have to reverse transcript. doh! */
	new_tr = Transcript_alloc_std();
	temp = out->start - out->end;
	for(i=tr->ex_len-1;i >= 0;i-- ) {
	  exon = Exon_alloc_std();
	  exon->start = temp - tr->exon[i]->end;
	  exon->end   = temp - tr->exon[i]->start;
	  fprintf(stderr,"Adding exon %d %d which used to be %d %d\n",exon->start,exon->end,tr->exon[i]->start,tr->exon[i]->end);
	  add_ex_Transcript(new_tr,exon);
	}
	free_Transcript(tr);
	tr = new_tr;
      }

      add_Gene(out,tr);

      ts->parent = tr;
      add_Transcript(tr,ts);
      tr->parent = out;
      
    } else {
      if( trace ) 
	fprintf(stderr,"Skipping %s before codon\n",alc->alu[1]->text_label);
      /* do nothing if not a codon - march on */
    }
    
  }
	
  /*  free_AlnBlock(coll);*/

  return 1;
}


/* Function:  is_exon_AlnColumn_new(alc)
 *
 * Descrip:    helper function for new system
 *
 *
 * Arg:        alc [UNKN ] Undocumented argument [AlnColumn *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
# line 321 "geneutil.dy"
boolean is_exon_AlnColumn_new(AlnColumn * alc) 
{
  if( strstr(alc->alu[1]->text_label,"CODON") != NULL ) {
    return TRUE;
  } else { 
    return FALSE;
  }

}


/* Function:  is_splice_site_AlnColumn(alc,type,phase)
 *
 * Descrip:    helper function for the new system
 *
 *
 * Arg:          alc [UNKN ] Undocumented argument [AlnColumn *]
 * Arg:         type [UNKN ] Undocumented argument [int *]
 * Arg:        phase [UNKN ] Undocumented argument [int *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
# line 335 "geneutil.dy"
boolean is_splice_site_AlnColumn(AlnColumn * alc,int * type,int * phase)
{

  if( strstr(alc->alu[1]->text_label,"SS") != NULL ) {

    if( strstr(alc->alu[1]->text_label,"5SS") != NULL ) {
      *type = 5;
    } else {
      *type = 3;
    }

    if( strstr(alc->alu[1]->text_label,"0") != NULL ) {
      *phase = 0;
    } else if ( strstr(alc->alu[1]->text_label,"1") != NULL ) {
      *phase = 1;
    } else if ( strstr(alc->alu[1]->text_label,"2") != NULL ) {
      *phase = 2;
    }
    return TRUE;
  } else {
    return FALSE;
  }
}

/* Function:  Gene_from_AlnColumn_new(alc,end)
 *
 * Descrip:    A new hope for building genes
 *
 *
 * Arg:        alc [UNKN ] Undocumented argument [AlnColumn *]
 * Arg:        end [UNKN ] Undocumented argument [AlnColumn **]
 *
 * Return [UNKN ]  Undocumented return value [Gene *]
 *
 */
# line 362 "geneutil.dy"
Gene * Gene_from_AlnColumn_new(AlnColumn * alc,AlnColumn ** end)
{
  Gene * out;
  Transcript * tr;
  Translation * ts;
  Exon * ex;

  int exon_start;
  int exon_end;
  int phase_start;
  int phase_end;
  
  int is_reversed;
  int is_3ss;

  int phase;
  int type;
  int tmp;

  AlnColumn * prev;

  boolean end_gene = 0;

  while(alc != NULL && is_exon_AlnColumn_new(alc) == FALSE )
    alc = alc->next;

  if( alc == NULL ) {
    *end = NULL;
    return NULL;
  }

  if( strstr(alc->alu[1]->text_label,"CODON") == NULL ) {
    warn("Bad news... exited from random columns, but not in a codon column, in a %s",alc->alu[1]->text_label);
    * end = NULL;
    return NULL;
  }

  /* we should have either a forward or reversed gene */

  if( strstr(alc->alu[1]->text_label,"REV") != NULL ) {
    is_reversed = 1;
  } else {
    is_reversed = 0;
  }

  out = Gene_alloc_len(1);
  tr = Transcript_alloc_std();
  add_Gene(out,tr);

  out->start = alc->alu[1]->start +1; 

  phase_start = 0;
  exon_start  = 0; /* by definition, the first exon is at the start*/
  is_3ss = 0;

  for(;;) {

    if( strstr(alc->alu[1]->text_label,"GENE_EXIT") != NULL ) {
      break;
    }

    exon_start = alc->alu[1]->start+1 - out->start;
    
    if( is_3ss ) {
      if( phase_start == 0 ) {
	exon_start -= 3;
      } else if ( phase_start == 1 ) {
	exon_start -= 2;
      } else if ( phase_start == 2 ) {
	exon_start -= 1;
      }
    }

    /* this is at the start of exon */
    /* go to the end of the exon */
    for(; alc != NULL && is_exon_AlnColumn_new(alc);alc=alc->next) {
      fprintf(stdout,"Exonifying past %d %s\n",alc->alu[1]->start,alc->alu[1]->text_label);
      prev = alc;
    }

    /* irregardless of the fate of this exon, we now can put it
     * in, as if it was phase0-phase0 */

    
    exon_end = prev->alu[1]->end +1 - out->start;

    
    ex = Exon_alloc_std();
    fprintf(stdout,"Adding exon with %d-%d\n",ex->start,ex->end);
    add_ex_Transcript(tr,ex);
    
    ex->start = exon_start;
    ex->end   = exon_end;

    if( alc != NULL && is_splice_site_AlnColumn(alc,&type,&phase) ) {
      phase_end = phase;
    } else {
      is_3ss = 0;
      phase_end = 0;
      end_gene = 1;
    }

    /* jigging the splice sites for the phases */

    if( phase_end == 0 ) {
      ex->end = ex->end;
    } else if( phase_end == 1 ) {
      ex->end = ex->end +1;
    } else if( phase_end == 2 ) {
      ex->end = ex->end +2;
    }

    ex->phase = phase_start;

    if( end_gene == 1 ) {
      break;
    }

    for(; alc != NULL && !is_exon_AlnColumn_new(alc);alc=alc->next) {
/*      fprintf(stdout,"Moving past %d %s\n",alc->alu[1]->start,alc->alu[1]->text_label);*/
      if( is_splice_site_AlnColumn(alc,&type,&phase) ) {
	is_3ss = 1;
	phase_start = phase;
      }
      if( strstr(alc->alu[1]->text_label,"GENE_EXIT") != NULL ) {
/*	fprintf(stdout,"Exiting %d %s\n",alc->alu[1]->start,alc->alu[1]->text_label); */
	end_gene = 1;
	break;
      }

    }

    if( end_gene == 1 ) {
      break;
    }

    if( alc == NULL ) {
      break;
    }
  }
  
  *end = alc;
  out->end = ex->end + out->start;

  if( is_reversed == 1 ) {
    tmp = out->start;
    out->start = out->end;
    out->end = tmp;
  }


  ts = Translation_alloc();
  ts->start = 0;
  ts->end = length_Transcript(tr);
  ts->parent = tr;  
  add_Transcript(tr,ts);

  tr->parent = out;


  fprintf(stdout,"returning gene %d %d\n",out->start,out->end);

  return out;
} 

/* Function:  Gene_from_AlnColumn_GeneWise(alc,org_start,org_end,predict_pseudo_for_frameshift,end)
 *
 * Descrip:    A wrap for making a gene structure from
 *             an AlnBlock derived from one of the genewise
 *             methods
 *
 *
 * Arg:                                  alc [UNKN ] Alignment column in an AlnBlock produced by genewise [AlnColumn *]
 * Arg:                            org_start [UNKN ] offset that the genewise alignment was to the coordinate system [int]
 * Arg:                              org_end [UNKN ] emd that the genewise alignment was to the coordinate system [int]
 * Arg:        predict_pseudo_for_frameshift [UNKN ] Undocumented argument [boolean]
 * Arg:                                  end [WRITE] pointer to a AlnColumn * to say when it has finished with this gene [AlnColumn **]
 *
 * Return [UNKN ]  Undocumented return value [Gene *]
 *
 */
# line 537 "geneutil.dy"
Gene * Gene_from_AlnColumn_GeneWise(AlnColumn * alc,int org_start,int org_end,boolean predict_pseudo_for_frameshift,AlnColumn ** end)
{
  Gene * out;
  Transcript * tr;
  Translation * ts;
  Exon * ex;
  AlnColumn * prev;
  SupportingFeature * sf;
  int score = 0; 
  int dosf = 0;
  int phase = 0;
  int frame_break = 0;


  while(alc != NULL && is_random_AlnColumn_genewise(alc) == TRUE )
    alc = alc->next;

  while (alc != NULL && strcmp(alc->alu[1]->text_label,"CODON") != 0 ) 
    alc = alc->next;

  if( alc == NULL)
    return NULL;

  out = Gene_alloc_len(1);
  tr = Transcript_alloc_std();
  add_Gene(out,tr);

  out->start = alc->alu[1]->start +1; 

  score += alc->alu[0]->score[0];

  for(;;) {

/*        fprintf(stderr,"Top loop - alc at %s %s\n",alc->alu[1]->text_label,alc->alu[0]->text_label);  */

    /*    fprintf(stderr,"2 Score is %.2f %s %d %.2f\n",Score2Bits(score),alc->alu[1]->text_label,alc->alu[0]->score[0],Score2Bits(alc->alu[0]->score[0]));*/
    ex = Exon_alloc_std();
    add_ex_Transcript(tr,ex);

    /*
     * this is always the start of an exon 
     */

    dosf = 0;
 
    if( strcmp(alc->alu[1]->text_label,"CODON") == 0 ) {      
      ex->start = alc->alu[1]->start+1 - out->start; /* coordinated in alignment coords */
      dosf = 1;
      phase = 0;
    } else if ( strcmp(alc->alu[1]->text_label,"3SS_PHASE_0") == 0  ) {
      ex->start = alc->alu[1]->start +4 - out->start; 
      dosf = 1;
      phase = 0;
    } else if ( strcmp(alc->alu[1]->text_label,"3SS_PHASE_1") == 0  ) {
      ex->start = alc->alu[1]->start +4 - out->start; 
      phase = 1;
    } else if ( strcmp(alc->alu[1]->text_label,"3SS_PHASE_2") == 0  ) {
      ex->start = alc->alu[1]->start +4 - out->start; 
      phase = 2;
    } else {
      ex->start = alc->alu[1]->start +1 - out->start; /* coordinated in alignment coords */
      phase = -1;
    }

    ex->phase = phase;

    /* 
     * Exons can start in INSERTs (yuk). In which case we don't 
     * make a supporting feature
     */


    if( dosf == 1 && strstartcmp(alc->alu[0]->text_label,"INSERT") != 0 ) {
      sf = SupportingFeature_alloc();
      /* we fill in start and end from the exon at the moment. Not pretty */
      sf->hstart  = alc->alu[0]->start+1;
      sf->hstrand = 1; /* currently only got proteins. Thank the lord! */
      sf->start = ex->start;
    } else {
      sf = NULL; /* make sure we don't generate a sf here */
    }
  


    for(prev=alc,alc=alc->next;alc != NULL; ) {
     /* fprintf(stderr,"Exon loop - alc at %s %s %d\n",alc->alu[1]->text_label,alc->alu[0]->text_label,sf); 	*/

      score += alc->alu[0]->score[0];
      /*      fprintf(stderr,"1 Score is %.2f %s\n",Score2Bits(score),alc->alu[1]->text_label); */
      if( is_frameshift_AlnColumn_genewise(alc) == TRUE && predict_pseudo_for_frameshift == TRUE ) {
	score += alc->alu[0]->score[0];
	fprintf(stderr,"Score is %.2f\n",Score2Bits(score));
	out->ispseudo = TRUE;
	alc = alc->next;
	continue;
      }

      if( is_random_AlnColumn_genewise(alc) == TRUE)
	break;

      if( strcmp(alc->alu[1]->text_label,"CODON") != 0 ) {
	if( strstartcmp(alc->alu[0]->text_label,"DELETE") == 0 ) {
	  /* must add sf and start a new one */
	  /*fprintf(stderr,"Looking at alc at %s %s %d %d %d\n",alc->alu[1]->text_label,alc->alu[0]->text_label,prev,out,sf);*/

	  if( sf != NULL ) {
	    sf->end  = prev->alu[1]->end+1 - out->start;
	    sf->hend = prev->alu[0]->end+1;
	    add_Exon(ex,sf);
	    sf = NULL;
	  }
	
	  /* 
	   * go the end of this delete run, which are residues in the query with no
	   * target info
	   */
	  while( alc->next != NULL && strstartcmp(alc->next->alu[0]->text_label,"DELETE") == 0 ) {
	    alc = alc->next;
	  }
	  if( alc->next != NULL && strcmp(alc->next->alu[1]->text_label,"CODON") == 0 ) {
	    /* the next position is the start of the new alignment */
	    sf = SupportingFeature_alloc();
	    sf->hstart = alc->next->alu[0]->start+1;
	    sf->start  = alc->next->alu[1]->start+1 - out->start;
	  } else {
	    sf = NULL;
	  }
	} else {
	  break;
	}
      
      } else {
	/* it is a codon match, but it could be an insert */
	if( strstartcmp(alc->alu[0]->text_label,"INSERT") == 0) {
	  /* break at this point, add this supporting feature */
	  if( sf != NULL ) {
	    sf->end  = prev->alu[1]->end+1 - out->start;
	    sf->hend = prev->alu[0]->end+1;
	    add_Exon(ex,sf);
	    sf = NULL;
	  }
	  
	  frame_break = 0;

	  /* go to the end of this insert run, watching for frameshifts */
	  while( alc->next != NULL && strstartcmp(alc->next->alu[0]->text_label,"INSERT") == 0 ) {

	    if( is_frameshift_AlnColumn_genewise(alc->next) == TRUE || is_random_AlnColumn_genewise(alc->next) == TRUE) {

	      if( is_frameshift_AlnColumn_genewise(alc->next) == TRUE && predict_pseudo_for_frameshift == TRUE ) {
		out->ispseudo = TRUE;
		alc = alc->next;
		continue;
	      } else {
		alc = alc->next;
		frame_break = 1;
		break;
	      }
	    }
	    alc = alc->next;
	  }
	  if( frame_break == 1 ) {
	    break; /* out of this gene */
	  }

	  if( alc->next != NULL && strcmp(alc->next->alu[1]->text_label,"CODON") == 0 ) {
	    /* the next position is the start of the new alignment */
	    sf = SupportingFeature_alloc();
	    /* do not understand why not having a +1 here is correct. Hmph */
	    sf->hstart = alc->next->alu[0]->start+1;
	    sf->start  = alc->next->alu[1]->start+1 - out->start;
	  } else {
	    sf = NULL;
	  }
	
	} else {
	  /* could be the start of a run from INSERT into match */
	  if( sf == NULL ) {
	    sf = SupportingFeature_alloc();
	    /* we fill in start and end from the exon at the moment. Not pretty */
	    sf->hstart  = alc->alu[0]->start+1;
	    sf->hstrand = 1; /* currently only got proteins. Thank the lord! */
	    sf->start = alc->alu[1]->start+1 - out->start;
	  }
	}
      } /* end of else it is a codon match */
      prev  = alc;
      alc = alc->next;
      
    }

    /*
     * The exon has ended. But why?
     */

    if( sf != NULL ) {
      sf->hend = prev->alu[0]->end+1;
      add_Exon(ex,sf);
    }

    if( alc == NULL ) {
      out->end = prev->alu[1]->end +1;
      ex->end = out->end - out->start;
      if( sf != NULL ) {
	sf->end = ex->end;
      }
      break;
    } 

    if( is_random_AlnColumn_genewise(alc) == TRUE) {
      out->end = prev->alu[1]->end +1;
      ex->end = out->end - out->start;
      if( sf != NULL ) {
	sf->end = ex->end;
      }
      break;
    }

   
   
/*    fprintf(stderr,"Exiting out of exon loop...\n");*/

    if( strcmp(alc->alu[1]->text_label,"5SS_PHASE_0") == 0 ) {
      out->end = alc->alu[1]->start+1;
      phase = 0;
    } else if ( strcmp(alc->alu[1]->text_label,"5SS_PHASE_1") == 0 ) {
      out->end = alc->alu[1]->start+2;
      phase = 1;
    } else if ( strcmp(alc->alu[1]->text_label,"5SS_PHASE_2") == 0 ) {
      out->end = alc->alu[1]->start+3;
      phase = 2;
    } else {
      phase = 0;
	
      /* fixes from Steve, via an issue with Steve */
      if ( strncmp(prev->alu[1]->text_label,"3SS_PHASE_0",11) == 0 ) {
        out->end = prev->alu[1]->end + 4;
      } else if ( strncmp(prev->alu[1]->text_label,"3SS_PHASE_1",11) == 0 ) {
        out->end = prev->alu[1]->end + 4;
      } else if ( strncmp(prev->alu[1]->text_label,"3SS_PHASE_1",11) == 0 ) {
        out->end = prev->alu[1]->end + 4;
      } else {
        out->end = prev->alu[1]->end +1;
      }	 

      ex->end = out->end - out->start;
      if( sf != NULL ) {
	sf->end = ex->end;
      }
      break;
      
    }


    /** set end of exon to the correct size from here **/
    ex->end = out->end - out->start;


    /** sf is to the codon, not to the exon */
    if( sf != NULL ) {
      if( phase == 0 ) {
	sf->end = ex->end;
      } else if ( phase == 1 ) {
	sf->end = ex->end-1;
      } else {
	sf->end = ex->end-2;
      }
    }
      


    while( alc != NULL && strstartcmp(alc->alu[1]->text_label,"3SS") != 0 ) {
/*      fprintf(stderr,"Intron loop - alc at %s %s %d\n",alc->alu[1]->text_label,alc->alu[0]->text_label,sf); 	*/
      alc = alc->next;
      score += alc->alu[0]->score[0];
    }

    if( alc == NULL ) {
      warn("Got to the end of an alignment inside an intron. Oooops!");
      break;
    }

  }/* back to for(;;) */

  
  if( end != NULL ) 
    *end = alc;

  if( org_start < org_end) {
    out->start += org_start-1;
    out->end   += org_start-1;
  } else {
    /*    fprintf(stderr,"was %d to %d\n",out->start,out->end); */
    out->start = org_start-1 - out->start;
    out->end   = org_start-1 - out->end;
    /*    fprintf(stderr,"now %d to %d (%d)\n",out->start,out->end,org_start); */
  }

  if( out->ispseudo == FALSE ) {
    ts = Translation_alloc();
    ts->start = 0;
    ts->end = length_Transcript(tr);
    ts->parent = tr;  
    add_Transcript(tr,ts);
  }

  tr->parent = out;
  
  out->bits = Score2Bits(score);
  return out;
}


/* Function:  is_frameshift_AlnColumn_genewise(alc)
 *
 * Descrip:    This function is to say what is a frameshift label
 *
 *
 * Arg:        alc [UNKN ] Undocumented argument [const AlnColumn *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
# line 854 "geneutil.dy"
boolean is_frameshift_AlnColumn_genewise(const AlnColumn * alc)
{
  if( strcmp(alc->alu[1]->text_label,"SEQUENCE_INSERTION") == 0 ) {
    return TRUE;
  }
  if( strcmp(alc->alu[1]->text_label,"SEQUENCE_DELETION") == 0 ) {
    return TRUE;
  }
  return FALSE;
}

/* Function:  is_random_AlnColumn_genewise(alc)
 *
 * Descrip:    This function is to say where this should
 *             be skipped in alignment/gene prediction problems
 *
 *
 * Arg:        alc [UNKN ] Undocumented argument [const AlnColumn *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
# line 870 "geneutil.dy"
boolean is_random_AlnColumn_genewise(const AlnColumn * alc)
{
  char * la;

  la  = alc->alu[1]->text_label;

  if( strcmp(la,"RANDOM_SEQUENCE") == 0 )
    return TRUE;
  if( strcmp(la,"END") == 0 )
    return TRUE;

  la  = alc->alu[0]->text_label;
  
  if( strstr(la,"_RND_") != NULL )
    return TRUE;

  return FALSE;
}


/* Function:  is_intron_AlnColumn_genewise(alc)
 *
 * Descrip:    This function is to say where things are introns
 *
 *
 * Arg:        alc [UNKN ] Undocumented argument [const AlnColumn *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
# line 894 "geneutil.dy"
boolean is_intron_AlnColumn_genewise(const AlnColumn * alc)
{
  char * la;

  la  = alc->alu[1]->text_label;

  if( strcmp(la,"CENTRAL_INTRON") == 0 )
    return TRUE;
  if( strcmp(la,"PYRIMIDINE_TRACT") == 0 )
    return TRUE;
  if( strcmp(la,"SPACER") == 0 )
    return TRUE;

  return FALSE;
}


#define GW_EXON_TYPE_UTR5 45
#define GW_EXON_TYPE_CDS  46
#define GW_EXON_TYPE_UTR3 47
#define GW_EXON_TYPE_NONE 48

# line 916 "geneutil.dy"
int  exon_type_AlnColumn_genomewise(AlnColumn * alc) 
{
  if( strcmp(alc->alu[1]->text_label,"CODON") == 0 ) {
    return GW_EXON_TYPE_CDS;
  }

  if( strcmp(alc->alu[1]->text_label,"UTR5") == 0 ) {
    return GW_EXON_TYPE_UTR5;
  }

  if( strcmp(alc->alu[1]->text_label,"UTR3") == 0 || strcmp(alc->alu[1]->text_label,"STOP_CODON") == 0) {
    return GW_EXON_TYPE_UTR3;
  }

  return GW_EXON_TYPE_NONE;
}


# line 934 "geneutil.dy"
void show_utr_exon_genomewise(AlnBlock * alb,FILE * ofp)
{
  AlnColumn * alc;
  int exon_start;
  int exon_end;
  int is_start;
  int phase;
  int endphase;
  int is_3ss;

  for(alc=alb->start;alc != NULL;) {
    /* find the first exon */
    for(;alc != NULL && exon_type_AlnColumn_genomewise(alc) != GW_EXON_TYPE_UTR5;alc = alc->next)
      ;
    if( alc == NULL ) {
      break;
    }
    fprintf(ofp,"Gene\n");

    if( alc != NULL && exon_type_AlnColumn_genomewise(alc) == GW_EXON_TYPE_UTR5 ) {
      is_start = 1;
      while( alc != NULL ) { /* while loop goes over all 5UTRs */
	exon_start = alc->alu[1]->start+ (is_start ? 2 : 3);
        is_start = 0;
	for(;alc != NULL && exon_type_AlnColumn_genomewise(alc) == GW_EXON_TYPE_UTR5;alc = alc->next ) {
	  ;
	} 
        /*	fprintf(stderr,"Broken out with %s\n",alc->alu[1]->text_label); */

	if( strcmp(alc->alu[1]->text_label,"UTR5_INTRON") ==0 ) {
	  /* ntron. should be +2-1 at the end of this, goes to 1*/
	  fprintf(ofp,"  utr5 %d %d\n",exon_start,alc->alu[1]->start+1);
	  /* now loop through the intron */
	  for(;alc != NULL && strcmp(alc->alu[1]->text_label,"UTR5_INTRON") == 0;alc = alc->next ) {
	    ;
	  }
	  if( alc == NULL || exon_type_AlnColumn_genomewise(alc) != GW_EXON_TYPE_UTR5 ) {
	    break; /* while loop */
	  } else{
	    continue; /* another utr5 exon */
	  }
	} else {
	  /* print this guy and break */
	  fprintf(ofp,"  utr5 %d %d\n",exon_start,alc->alu[1]->start+1);
	  break;
	}
      }
    }


    /* we now should be at a CDS column */

    if( alc != NULL && exon_type_AlnColumn_genomewise(alc) == GW_EXON_TYPE_CDS ) {
      is_start = 1;
      while( alc != NULL ) { /* while loop goes over all coding Exons */
	/* fprintf(stderr,"Entering codoing loop with %s\n",alc->alu[1]->text_label); */

	exon_start = alc->alu[1]->start+2;
	if( strstr(alc->alu[1]->text_label,"3SS") != NULL ) {
	  is_3ss = 1;
	  if( strstr(alc->alu[1]->text_label,"1") != NULL ) {
	    phase = 1;
	  } else if ( strstr(alc->alu[1]->text_label,"2") != NULL ) {
	    phase = 2;
	  } else {
	    phase = 0;
	  }
	  alc = alc->next;
	} else {
	  is_3ss = 0;
	  phase = 0;
	}

	
	if( phase == 1 ) {
	  exon_start += 3;
	} else if ( phase == 2) {
	  exon_start += 3;
	} else if ( is_3ss ) {
	  /* phase 0 and spliced needs adjusting */
	  exon_start += 3;
	} 

	  

	for(;alc != NULL && exon_type_AlnColumn_genomewise(alc) == GW_EXON_TYPE_CDS ;alc = alc->next ) {
	  ;
	} 

	if( strstr(alc->alu[1]->text_label,"5SS") != NULL ) {

	  exon_end = alc->alu[1]->start+1;

	  if( strstr(alc->alu[1]->text_label,"1") != NULL ) {
	    endphase = 1;
	  } else if ( strstr(alc->alu[1]->text_label,"2") != NULL ) {
	    endphase = 2;
	  } else {
	    endphase = 0;
	  }

	  if( endphase == 1 ) {
	    exon_end += 1;
	  } else if( endphase == 2 ) {
	    exon_end += 2;
	  } /* no change for phase 0 */
	  
	  /* intron. should be +1-1 at the end of this, goes to 0*/
	  fprintf(ofp,"  cds %d %d phase %d\n",exon_start,exon_end,phase);

	  /* now loop through the intron */
	  for(alc= alc->next;alc != NULL && strstr(alc->alu[1]->text_label,"3SS") == NULL;alc = alc->next ) {
	    ;
	  }
	  if( alc == NULL || strstr(alc->alu[1]->text_label,"3SS") == NULL ) {
	    break; /* while loop */
	  } else{
	    continue; /* another cds exon */
	  }
	} else {
	  fprintf(ofp,"  cds %d %d phase %d\n",exon_start,alc->alu[1]->start+1,phase);
	  break;
	}
      }
    }

   
    if( alc != NULL && exon_type_AlnColumn_genomewise(alc) == GW_EXON_TYPE_UTR3 ) {
      is_start = 1;
      while( alc != NULL ) { /* while loop goes over all 3UTRs */
	exon_start = alc->alu[1]->start+ (is_start ? 2 : 3);
        is_start = 0;
	for(;alc != NULL && exon_type_AlnColumn_genomewise(alc) == GW_EXON_TYPE_UTR3 ;alc = alc->next ) {
	  ;
	} 


	if( strstr(alc->alu[1]->text_label,"INTRON") != NULL ) {
	  /* intron. should be +2-1 at the end of this, goes to 1*/
	  fprintf(ofp,"  utr3 %d %d\n",exon_start,alc->alu[1]->start+1);
	  /* now loop through the intron */
	  for(;alc != NULL && strstr(alc->alu[1]->text_label,"INTRON") != NULL;alc = alc->next ) {
	    ;
	  }
	  if( alc == NULL || exon_type_AlnColumn_genomewise(alc) != GW_EXON_TYPE_UTR3 ) {
	    break; /* while loop */
	  } else{
	    continue; /* another utr3 exon */
	  }
	} else {
	  fprintf(ofp,"  utr3 %d %d\n",exon_start,alc->alu[1]->start+1);
	  break;
	}
      }
    }

    fprintf(ofp,"End\n");
    /* back to next gene */

  }


}


# line 1149 "geneutil.c"

#ifdef _cplusplus
}
#endif
