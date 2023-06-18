
void set_union(set_t *s, const set_t *a, const set_t *b)
{
     for (unsigned i = 0; i < a->card; i++)
    {
        for (unsigned j = 0; j < b->card; j++)
        {
           if(b->items[j]==a->items[i]){
               for ( i = 0; i <  s->card; i++)        //osetrenie v pripade ze sa prvok v mnozine uz raz nachadza
         
                {
                    if(b->items[j]!=s->items[j]);
                    s->card++;
                    s->items[s->card]+= a->items[j];
                }    
               
           }
        
        }
        
    }
    
    return;
}


void matrix_add(matrix_t* m1, matrix_t* m2){
    if((m1->rows==m2->rows)&&(m1->colls==m2->colls)){
        for (unsigned r = 0; r < m1->rows; r++){
            for (unsigned c = 0; c < m1->colls; c++){
                m1->data[r][c]+=m2->data[r][c];
                             
                 
           
    
             }
        }
    }
    else
    {
        fprintf(stderr,"chyba matice nemaju rovnaky pocet stlpcov alebo riadkov");
    }
    
    
    


}

float vector_scalarMult(vector_t* a,vector_t* b){
    float sucin=0;
    if(a->length==b->length){
        for (unsigned i = 0; i < a->length; i++){
        sucin+= a->data[i] * b->data[i];
        }
    
        
    }
    else{
        fprintf(stderr,"vektory nemaju rovnake dlzky");
        return 0;
    }
    
    
    


return sucin;
}

