
camera{
   location  <278, 273,-530>
   look_at <278, 273, -529>
   up        <0, 1, 0>
   right     <-1, 0, 0> // width and height must be equals !
   // angle 39.5
} 


//#declare light_color = color rgb<0.780131,0.780409,0.775833>

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
  
  DiffuseMaterial {
    Kd <0.740063,0.742313,0.733934> 
  }
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
  
  DiffuseMaterial {
    Kd <0.162928,0.408903,0.0833759> 
    }
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

  DiffuseMaterial{ 
    Kd <0.366046,0.0371827,0.0416385>
  }  
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

  DiffuseMaterial {
    Kd <0.740063,0.742313,0.733934> 
  }
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
    <472.0, 330.0, 406.0>
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
    <314.0, 0.0, 456.0>
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
  
  DiffuseMaterial {
    Kd <0.740063,0.742313,0.733934> 
  }
}

area_light {

    triangle {
      <213,548.7,332>, <213, 548.7, 227>, <343, 548.7, 227>
      DiffuseMaterial {
        Kd <1.0, 1.0, 1.0>
      }
    }
  power 15
  color <1, 1, 1>
}


area_light {
   triangle {
      <343,548.7,332>, <213, 548.7, 332>, <343, 548.7, 227>
      DiffuseMaterial {
        Kd <1.0, 1.0, 1.0>
      }
    }
  
  power 15
  color <1, 1, 1>
}