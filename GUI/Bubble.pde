class ZObject {
  float x, y, z, xsize, ysize;
  color bubble_color;
  color shaded_color;
  float vx, vy, vz;
 
  ZObject(float ix, float iy, float iz, color icolor) {
    x = ix;
    y = iy;
    z = iz;
    xsize = RADIUS;
    ysize = RADIUS;
    bubble_color = icolor;
    setColor();
    vx = random(-1.0, 1.0);
    vy = random(-1.0, 1.0);
    vz = random(-1.0, 1.0);
    float magnitude = sqrt(vx*vx + vy*vy + vz*vz);
    vx = SPEED * vx / magnitude;
    vy = SPEED * vy / magnitude;
    vz = SPEED * vz / magnitude;
  }
  
  void setColor() {
    float shade = z;
    float shadeinv = 1.0-shade;
    shaded_color = color( (red(bubble_color)*shade)+(red(BACKGROUND)*shadeinv),
                    (green(bubble_color)*shade)+(green(BACKGROUND)*shadeinv),
                    (blue(bubble_color)*shade)+(blue(BACKGROUND)*shadeinv));
  }
  
  void draw(float xoffs, float yoffs) {
    float posX = (x*WIDTH*(1+z*z)) - xoffs*WIDTH*z*z;
    float posY = (y*HEIGHT*(1+z*z)) - yoffs*HEIGHT*z*z;
    float radius = z*xsize;
    if (posX> -xsize*2 && posX < WIDTH+xsize*2 && posY > -xsize*2 && posY < HEIGHT+xsize*2) {
        blurred_circle(posX, posY, radius, abs(z-FOCAL_LENGTH), shaded_color, MIN_BLUR_LEVELS + (z*BLUR_LEVEL_COUNT));
    }
  }
}

void sortBubbles() {
   
    float last = 0;
    ArrayList temp = new ArrayList();
    for (int i=0; i<N; i++) {
        int index = 0;
        float lowest = 100.0;
        for (int j=0; j<N; j++) {
            ZObject current = (ZObject)objects.get(j);
            if (current.z < lowest && current.z > last) {
                index = j;
                lowest = current.z;
            }
        }
        temp.add(objects.get(index));
        last = ((ZObject)objects.get(index)).z;
    }
    objects = temp;
}

void blurred_circle(float x, float y, float rad, float blur, color col, float levels) {
    float level_distance = BLUR_AMOUNT*(simY()/200)/levels;
    for (float i=0.0; i<levels*2; i++) {
      fill(col, 255*(levels*2-i)/(levels*2));
      ellipse(x, y, rad+(i-levels)*level_distance, rad+(i-levels)*level_distance);
    }
}

float simX(){
  int period = 30;
  return 300 * cos(360 * radians(millis()) / 1000 / period) + 500;
}

float simY(){
  int period = 10;
  return 30 * sin(360 * radians(millis()) / 1000 / period) + 80;
}
