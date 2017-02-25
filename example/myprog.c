
float add(float a, float b) {
  return a + b;
}

typedef struct Point {
  int x;
  int y;
} Point;

void point_add_to(Point a, Point b) {
  a.x += b.x;
  a.y += b.y;
}


