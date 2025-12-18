#include <iostream>;
#include <cmath>;
namespace chernikov {
  struct p_t {
    double x;
    double y;
  };
  struct r_t {
    double w;
    double h;
    p_t pos;
  };
  struct IShape {
    virtual void scale(double ct) = 0;
    virtual ~IShape() = default;
    virtual r_t getFrameRectangle() const = 0;
    virtual void move(p_t pt) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual double getArea() const = 0;
  };
  struct Rectangle : IShape {
    Rectangle(double w, double h, p_t mid);
    void move(double dx, double dy) override;
    void move(p_t pt) override;
    double getArea() const override;
    r_t getFrameRectangle() const override;
    void scale(double ct) override;
  private:
    double w, h;
    p_t mid;
  };
  struct Polygon : IShape {
    Polygon(size_t size, p_t *points);
    void move(double dx, double dy) override;
    void move(p_t pt) override;
    double getArea() const override;
    r_t getFrameRectangle() const override;
    void scale(double ct) override;
    ~Polygon();
  private:
    size_t size;
    p_t *pts;
    p_t mid;
  };
  struct Diamond : IShape {
    Diamond(double rad, p_t mid);
    void move(double dx, double dy) override;
    void move(p_t pt) override;
    double getArea() const override;
    r_t getFrameRectangle() const override;
    void scale(double ct) override;
    ~Diamond();
  private:
    double rad;
    p_t mid;
  };
  std::ostream &chernikov::output(std::ostream &os, IShape **shps, size_t size);
  void chernikov::scaleFromPoint(IShape **shps, size_t size, p_t point, double ct);
  chernikov::r_t chernikov::getTotalFrame(IShape **shps, size_t size);
}
int main() {
  chernikov::Rectangle r(10, 2, {6, 8});
  chernikov::p_t points[4] = {{0, 1}, {4, 6}, {3, 3}, {9.4, 3.3}};
  chernikov::Diamond d(5.2, {5, 5});
  try {
    chernikov::Polygon p(4, points);
    chernikov::IShape *shps[3] = {std::addressof(r), std::addressof(d), std::addressof(p)};
    chernikov::output(std::cout, shps, 3);
    chernikov::p_t point = {};
    double ct = 0;
    if (!(std::cin >> point.x >> point.y >> ct)) {
      std::cerr << "failed to count input" << "\n";
    return 1;
    }
    if (ct <= 0) {
      std::cerr << "Negative coef" << "\n";
    return 1;
    }
    chernikov::scaleFromPoint(shps, 3, point, ct);
    chernikov::output(std::cout, shps, 3);
    } catch (std::bad_alloc &e) {
      std::cerr << "failed to allocate memory" << "\n";
    return 2;
  }
  return 0;
}

chernikov::Rectangle::Rectangle(double w, double h, p_t mid) :
  IShape(), w(w), h(h), mid(mid)
  {}
void chernikov::Rectangle::move(p_t pt) {
  mid = pt;
}
void chernikov::Rectangle::move(double dx, double dy) {
  mid.x += dx;
  mid.y += dy;
}
double chernikov::Rectangle::getArea() const {
  return w * h;
}
chernikov::r_t chernikov::Rectangle::getFrameRectangle() const {
  return {w, h, mid};
}
void chernikov::Rectangle::scale(double ct) {
  w = w * ct;
  h = h * ct;
}

chernikov::Diamond::Diamond(double rad, p_t mid) :
  IShape(), rad(rad), mid(mid)
  {}
void chernikov::Diamond::move(p_t pt) {
  mid = pt;
}
void chernikov::Diamond::move(double dx, double dy) {
  mid.x += dx;
  mid.y += dy;
}
double chernikov::Diamond::getArea() const {
  return ((2 * rad) * (2 * rad)) / 2;
}
chernikov::r_t chernikov::Diamond::getFrameRectangle() const {
  return {2 * rad, 2 * rad, mid};
}
void chernikov::Diamond::scale(double ct) {
  rad = rad * ct;
}

chernikov::Polygon::Polygon(size_t size, p_t *points) :
  IShape(), size(size), pts(size < 3 ? nullptr : new p_t[size]), mid{0, 0} {
  if (size < 3) {
    throw std::logic_error("Not a polygon");
  }
  double area = 0;
  for (size_t i = 0; i < size; ++i) {
    pts[i] = points[i];
    size_t j = (i + 1) % size;
    area += points[i].x * points[j].y;
    area -= points[j].x * points[i].y;
  }
  area = std::abs(area) / 2;
  double mod = 0.0;
  for (size_t i = 0; i < size; ++i) {
    size_t j = (i + 1) % size;
    mod = (pts[i].x * pts[j].y - pts[j].x * pts[i].y);
    mid.x += (pts[i].x + pts[j].x) * mod;
    mid.y += (pts[i].y + pts[j].y) * mod;
  }
  mid.x /= (6.0 * area);
  mid.y /= (6.0 * area);
}
void chernikov::Polygon::move(double dx, double dy) {
  for (size_t i = 0; i < size; ++i) {
    pts[i].x += dx;
    pts[i].y += dy;
  }
  mid.x += dx;
  mid.y += dy;
}
void chernikov::Polygon::move(p_t pt) {
  double dx = pt.x - mid.x;
  double dy = pt.y - mid.y;
  this->move(dx, dy);
}
double chernikov::Polygon::getArea() const {
  double area = 0;
  for (size_t i = 0; i < size; ++i) {
    size_t j = (i + 1) % size;
    area += pts[i].x * pts[j].y;
    area -= pts[j].x * pts[i].y;
  }
  area = std::abs(area) / 2;
  return area;
}
chernikov::r_t chernikov::Polygon::getFrameRectangle() const {
  double maxx = pts[0].x, minx = pts[0].x;
  double maxy = pts[0].y, miny = pts[0].y;
  for (size_t i = 1; i < size; ++i) {
    maxx = std::max(maxx, pts[i].x);
    minx = std::min(minx, pts[i].x);
    maxy = std::max(maxy, pts[i].y);
    miny = std::min(miny, pts[i].y);
  }
  double w = maxx - minx;
  double h = maxy - miny;
  return {w, h, {maxx - w / 2, maxy - h / 2}};
}
void chernikov::Polygon::scale(double ct)
{
  --ct;
  double dx = 0, dy = 0;
  for (size_t i = 0; i < size; ++i) {
    dx = pts[i].x - mid.x;
    dy = pts[i].y - mid.y;
    pts[i].x += dx * ct;
    pts[i].y += dy * ct;
  }
}
std::ostream &chernikov::output(std::ostream &out, IShape **shps, size_t size) {
  double totalArea = 0.0;
  for (size_t i = 0; i < size; ++i) {
    double area = shps[i]->getArea();
    r_t frame = shps[i]->getFrameRectangle();
    totalArea += area;
    out << area << "\n";
    out << frame.pos.x << frame.pos.y << "\n";
    out << frame.w << frame.h << "\n";
  }
  r_t totalFrame = getTotalFrame(shps, size);
  out << totalArea << "\n";
  out << totalFrame.pos.x << totalFrame.pos.y << "\n";
  out << totalFrame.w << totalFrame.h << "\n";
  return out;
}
void chernikov::scaleFromPoint(IShape **shps, size_t size, p_t point, double ct) {
  for (size_t i = 0; i < size; ++i) {
    shps[i]->move(-point.x, -point.y);
    shps[i]->scale(ct);
    shps[i]->move(point.x, point.y);
  }
}
chernikov::r_t chernikov::getTotalFrame(IShape **shps, size_t size) {
  if (size == 0) {
    return {0.0, 0.0, {0.0, 0.0}};
  }
  r_t frame = shps[0]->getFrameRectangle();
  double left = frame.pos.x - frame.w / 2.0;
  double right = frame.pos.x + frame.w / 2.0;
  double bottom = frame.pos.y - frame.h / 2.0;
  double top = frame.pos.y + frame.h / 2.0;
  for (size_t i = 1; i < size; ++i) {
    frame = shps[i]->getFrameRectangle();
    double l = frame.pos.x - frame.w / 2.0;
    double r = frame.pos.x + frame.w / 2.0;
    double b = frame.pos.y - frame.h / 2.0;
    double t = frame.pos.y + frame.h / 2.0;
    if (l < left) {
      left = l;
    }
    if (r > right) {
      right = r;
    }
    if (b < bottom) {
      bottom = b;
    }
    if (t > top) {
      top = t;
    }
  }
  r_t total;
  total.w = right - left;
  total.h = top - bottom;
  total.pos.x = (left + right) / 2.0;
  total.pos.y = (bottom + top) / 2.0;
  return total;
}