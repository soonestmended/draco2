
#declare pov_rad=no;

global_settings
{
  ambient_light 0.01
#if(pov_rad)        
  ini_option "+QR"
  radiosity
  { pretrace_start 0.08
    pretrace_end   0.02
    count 100
    nearest_count 5
    error_bound 1
    recursion_limit 4
    low_error_factor .5
    gray_threshold 0.0
    minimum_reuse 0.015
    brightness 1.3
    adc_bailout 0.01/2
  }
#else
  stochastic_global_illumination { samples 400 distribution 2 }
#end
}

camera{
   location  <278, 273,-800>
   direction <0, 0, 1>
   up        <0, 1, 0>
   right     <-1, 0, 0> // width and height must be equals !
   angle 39.5
} 

/*
 * colors converted from spectral space.
 */
#declare white_cornell = texture {
    finish{ diffuse 1 }
    pigment { color rgb<0.740063,0.742313,0.733934> }
}

#declare green_cornell = texture {
    finish{ diffuse 1 }
    pigment { color rgb<0.162928,0.408903,0.0833759> }
}

#declare red_cornell = texture {
    finish{ diffuse 1 }
    pigment { color rgb<0.366046,0.0371827,0.0416385> }
}

#declare light_color = color rgb<0.780131,0.780409,0.775833>

union {
/*
 * floor
 */
  triangle {
    <552.8, 0.0, 0.0>,
    <0.0, 0.0, 0.0>,
    <0.0, 0.0, 559.2>
  }
  triangle {
    <552.8, 0.0, 0.0>,
    <0.0, 0.0, 559.2>,
    <549.6, 0.0, 559.2>
  }
/*
 * ceiling
 */
  triangle {
    <556.0, 548.8, 0.0>,
    <556.0, 548.8, 559.2>,
    <0.0, 548.8, 559.2>
  }
  triangle {
    <556.0, 548.8, 0.0>,
    <0.0, 548.8, 559.2>,
    <0.0, 548.8, 0.0>
  }
/*
 * back wall
 */
  triangle {
    <0.0, 548.8, 559.2>,
    <556.0, 548.8, 559.2>,
    <549.6, 0.0, 559.2>
  }
  triangle {
    <0.0, 548.8, 559.2>,
    <549.6, 0.0, 559.2>,
    <0.0, 0.0, 559.2>
  }
  
  texture { white_cornell }
}

union {
/*
 * right wall
 */
  triangle {
    <0.0, 548.8, 0.0>,
    <0.0, 548.8, 559.2>,
    <0.0, 0.0, 559.2>
  }
  triangle {
    <0.0, 548.8, 0.0>,
    <0.0, 0.0, 559.2>,
    <0.0, 0.0, 0.0>
  }
  
  texture { green_cornell }
}

union {
/*
 * left wall
 */
  triangle {
    <552.8, 0.0, 0.0>,
    <549.6, 0.0, 559.2>,
    <556.0, 548.8, 559.2>
  }
  triangle {
    <552.8, 0.0, 0.0>,
    <556.0, 548.8, 559.2>,
    <556.0, 548.8, 0.0>
  }

  texture { red_cornell }  
}

union {
/*
 * short block
 */
  triangle {
    <130.0, 165.0, 65.0>,
    <82.0, 165.0, 225.0>,
    <240.0, 165.0, 272.0>
  }
  triangle {
    <130.0, 165.0, 65.0>,
    <240.0, 165.0, 272.0>,
    <290.0, 165.0, 114.0>
  }

  triangle {
    <290.0, 0.0, 114.0>,
    <290.0, 165.0, 114.0>,
    <240.0, 165.0, 272.0>
  }
  triangle {
    <290.0, 0.0, 114.0>,
    <240.0, 165.0, 272.0>,
    <240.0, 0.0, 272.0>
  }

  triangle {
    <130.0, 0.0, 65.0>,
    <130.0, 165.0, 65.0>,
    <290.0, 165.0, 114.0>
  }
  triangle {
    <130.0, 0.0, 65.0>,
    <290.0, 165.0, 114.0>,
    <290.0, 0.0, 114.0>
  }

  triangle {
    <82.0, 0.0, 225.0>,
    <82.0, 165.0, 225.0>,
    <130.0, 165.0, 65.0>
  }
  triangle {
    <82.0, 0.0, 225.0>,
    <130.0, 165.0, 65.0>,
    <130.0, 0.0, 65.0>
  }

  triangle {
    <240.0, 0.0, 272.0>,
    <240.0, 165.0, 272.0>,
    <82.0, 165.0, 225.0>
  }
  triangle {
    <240.0, 0.0, 272.0>,
    <82.0, 165.0, 225.0>,
    <82.0, 0.0, 225.0>
  }

  texture { white_cornell }
}

union {
/*
 * tall block
 */
  triangle {
    <423.0, 330.0, 247.0>,
    <265.0, 330.0, 296.0>,
    <314.0, 330.0, 456.0>
  }
  triangle {
    <423.0, 330.0, 247.0>,
    <314.0, 330.0, 456.0>,
    <472.0 330.0 406.0>
  }

  triangle {
    <423.0, 0.0, 247.0>,
    <423.0, 330.0, 247.0>,
    <472.0, 330.0, 406.0>
  }
  triangle {
    <423.0, 0.0, 247.0>,
    <472.0, 330.0, 406.0>,
    <472.0, 0.0, 406.0>
  }

  triangle {
    <472.0, 0.0, 406.0>,
    <472.0, 330.0, 406.0>,
    <314.0, 330.0, 456.0>
  }
  triangle {
    <472.0, 0.0, 406.0>,
    <314.0, 330.0, 456.0>,
    <314.0, 0.0 456.0>
  }

  triangle {
    <314.0, 0.0, 456.0>,
    <314.0, 330.0, 456.0>,
    <265.0, 330.0, 296.0>
  }
  triangle {
    <314.0, 0.0, 456.0>,
    <265.0, 330.0, 296.0>,
    <265.0, 0.0, 296.0>
  }

  triangle {
    <265.0, 0.0, 296.0>,
    <265.0, 330.0, 296.0>,
    <423.0, 330.0, 247.0>
  }
  triangle {
    <265.0, 0.0, 296.0>,
    <423.0, 330.0, 247.0>,
    <423.0, 0.0, 247.0>
  }
  
  texture { white_cornell }
}

union {
/*
 * light patch
 */
  light_source {
    <278,548.8,279.5>
    light_color
    area_light <130, 0, 0>, <0, 0, 105>, 5, 5
    adaptive 1
    jitter
  }
  box { <213,548.7,332><343,548.8,227> no_shadow pigment { light_color }
finish { ambient light_color*1000 } }
}