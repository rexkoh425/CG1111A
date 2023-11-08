float filter_dist(float dist){

  float filtered_dist = 0 ;
  float ratio = 0.05;//current weightage
  
  if(abs(dist - previous_filtered_dist) > 2.0){//was2.5

    filtered_dist = dist;

  }else{

    filtered_dist = (1-ratio) * previous_filtered_dist + ratio * dist;
  
  } 

  previous_filtered_dist = filtered_dist;
  return filtered_dist;
  
}