namespace geometry{
    using D = long double;
    constexpr D eps =1e-9;

    struct Point;
    bool near_eq(Point, Point);
    D norm(Point);

    struct Point{
        D x, y;
        Point(D x = 0.0, D y = 0.0) : x(x), y(y){}
        friend bool operator<(const Point& a, const Point& b){
            return a.x == b.x ? a.y < b.y : a.x < b.x;
        }
        Point& operator+=(Point a){x += a.x, y += a.y; return *this;}
        Point& operator-=(Point a){x -= a.x, y -= a.y; return *this;}
        Point& operator*=(D p){x *= p, y *= p; return *this;}
        Point& operator*=(Point b){return *this = *this * b;}
        Point& operator/=(D p){x /= p, y /= p; return *this;}
        Point& operator/=(Point b){return *this = *this / b;}
        friend Point operator+(Point a, Point b){return Point(a) += b;}
        friend Point operator-(Point a, Point b){return Point(a) -= b;}
        friend Point operator*(Point a, D p){return Point(a) *= p;}
        friend Point operator*(Point a, Point b){return Point(a.x * b.x - a.y * b.y, a.x * b.y + b.x * a.y);}
        friend Point operator/(Point a, D b){return Point(a) /= b;}
        friend Point operator/(Point a, Point b){return Point(a.x * b.x + a.y * b.y, b.x * a.y - a.x * b.y) / norm(b);}
    };
    using P = Point;

    struct Circle : public Point{
        D r;
        Circle(Point p = Point(), D r = 1) : Point(p), r(r){}
        Circle(D x = 0.0, D y = 0.0, D r = 1) : Point(x, y), r(r){}
    };
    using C = Circle;

    bool near_eq(D a, D b = 0.0){return abs(a - b) < eps;}
    bool near_eq(P a, P b = Point()){return near_eq(a.x, b.x) && near_eq(a.y, b.y);}
    D diag(P a){
        assert(!near_eq(a));
        return atan2(a.y, a.x);
    }
    D norm(P a){return a.x * a.x + a.y * a.y;}
    D abs(P a){return sqrt(norm(a));}
    D dist(P a, P b){return abs(a - b);}
    D dot(P a, P b){return a.x * b.x + a.y * b.y;}
    D cross(P a, P b){return a.x * b.y - a.y * b.x;}
    int ccw(P a, P b, P c){
        b -= a;
        c -= a;
        if(cross(b, c) > eps)return 1;
        if(cross(b, c) < -eps)return -1;
        if(dot(b, c) < -eps)return 2;
        if(norm(b) < norm(c))return -2;
        return 0;
    }
    bool is_on_line(P a1, P a2, P b){return abs(ccw(a1, a2, b)) != -1;}
    bool is_on_segment(P a1, P a2, P b){return !ccw(a1, a2, b);}
    P proj(P a1, P a2, P b){return a1 + dot(a2 - a1, b - a1) / norm(a2 - a1) * (a2 - a1);} // 直線への射影点
    D dist(P a1, P a2, P b){return dist(proj(a1, a2, b), b);}
    bool intersect(P a1, P a2, P b1, P b2){
        return ccw(a1, a2, b1) * ccw(a1, a2, b2) <= 0 &&
               ccw(b1, b2, a1) * ccw(b1, b2, a2) <= 0;
    }
    P cross_point(P a1, P a2, P b1, P b2){
        D d1 = cross(b2 - b1, b1 - a1);
        D d2 = cross(b2 - b1, a2 - a1);
        if(near_eq(d1) && near_eq(d2))return a1;
        assert(!near_eq(d2));
        return a1 + d1 / d2 * (a2 - a1);
    }
    vector<Point> cross_point(C c1, C c2){
        vector<Point> cross;
        P diff = c2 - c1;
        D d = abs(diff);
        D crl = (norm(diff) + c1.r * c1.r - c2.r * c2.r) / (2 * d);
        if(near_eq(d) || c1.r < abs(crl))
            return cross;
        P abn = diff * P(0, sqrt(c1.r * c1.r - crl * crl) / d);
        P cp = c1 + crl / d * diff;
        cross.push_back(cp + abn);
        if(!near_eq(abn))
            cross.push_back(cp - abn);
        return cross;
    }
    vector<pair<P, P>> tangent_lines(C c1, C c2){ // 共通接線、接線の両端は円との接点
        vector<pair<P, P>> lines;
        D d = dist(c1, c2);
        for(int i = 0; i < 2; ++i){
            D sin =(c1.r - (1 - i * 2) * c2.r) / d;
            if(!(sin * sin < 1 + eps))
                break;
            D cos = sqrt(max(1 - sin * sin, D(0)));
            for(int j = 0; j < 2; ++j){
                P n = (c2 - c1) * P(sin, (1 - j * 2) * cos) / d;
                lines.emplace_back(c1 + c1.r * n, c2 + (1 - i * 2)  * c2.r * n);
                if(cos < eps)
                    break;
            }
        }
        return lines;
    }
}
