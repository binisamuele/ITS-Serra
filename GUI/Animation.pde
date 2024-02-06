class Animation {
  PImage[] images;
  int imageCount;
  int frame;
  
  Animation(int count) {
    imageCount = count;
    images = new PImage[imageCount];

    for (int i = 11, j=0; i >= 0; i--, j++) { 
      String filename = "frame_" + nf(i, 2) + "_delay-0.12s.gif";
      images[j] = loadImage(filename);
    }
  }

  void display(float xpos, float ypos) {
    frame = (frame+1) % imageCount;
    images[frame].resize(20, 20);
    image(images[frame], xpos, ypos);
    image(images[frame], xpos+350, ypos);
    image(images[frame], xpos+730, ypos);
  }
  
  int getWidth() {
    return images[0].width;
  }
}
