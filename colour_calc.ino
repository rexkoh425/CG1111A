char *Colour_calc(int red, int green, int blue) {
 if (red > 230 && green > 230 && blue > 230)
 {
  return "white";
 }

 if (red < 50 && green < 50 && blue < 50)
 {
  return "black";
 }
 if(red > green){

    if(green > blue){

      if(green > 110 && blue > 110){

        return "orange";
      }
      return "red";

    }else{

      return "purple";
    }

  }else if(blue > green){

      return "blue";

  }else{

    return "green";
  }
}