#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

struct Point {
    double x;
    double y;

    Point(double x_val, double y_val) : x(x_val), y(y_val) {}
};

class Polygon {
private:
    std::vector<Point> vertices;

    bool isClockwise() const {
        double sum = 0.0;
        int n = vertices.size();
        for (int i = 0; i < n; ++i) {
            int j = (i + 1) % n;
            sum += (vertices[j].x - vertices[i].x) * (vertices[j].y + vertices[i].y);
        }
        return sum > 0;
    }

public:
    void addVertex(double x, double y) {
        vertices.emplace_back(x, y);
    }

    void ensureCounterClockwise() {
        if (isClockwise()) {
            std::reverse(vertices.begin(), vertices.end());
        }
    }

    double area() const {
        double A = 0.0;
        int n = vertices.size();
        for (int i = 0; i < n; ++i) {
            int j = (i + 1) % n;
            A += vertices[i].x * vertices[j].y - vertices[j].x * vertices[i].y;
        }
        return std::abs(A) / 2.0;
    }

    Point centroid() const {
        double Cx = 0.0, Cy = 0.0;
        double A = area();
        int n = vertices.size();
        for (int i = 0; i < n; ++i) {
            int j = (i + 1) % n;
            double factor = (vertices[i].x * vertices[j].y - vertices[j].x * vertices[i].y);
            Cx += (vertices[i].x + vertices[j].x) * factor;
            Cy += (vertices[i].y + vertices[j].y) * factor;
        }
        Cx /= (6.0 * A);
        Cy /= (6.0 * A);
        return Point(Cx, Cy);
    }

    std::vector<Point> translateToCG() const {
        Point cg = centroid();
        std::vector<Point> translatedVertices;
        int n = vertices.size();

        for (size_t i = 0; i < n; ++i) {
            const Point& vertex = vertices[i];
            double translatedX = vertex.x - cg.x;
            double translatedY = vertex.y - cg.y;
            translatedVertices.emplace_back(translatedX, translatedY);
        }

        return translatedVertices;
    }

    void cutByHorizontalLine(double yCut) {
        std::vector<Point> newVertices;
        int n = vertices.size();

        for (int i = 0; i < n; ++i) {
            int j = (i + 1) % n;

            if (vertices[i].y < yCut && vertices[j].y >= yCut) {
                // Intersection with horizontal line
                double xIntersect = vertices[i].x + (yCut - vertices[i].y) * (vertices[j].x - vertices[i].x) / (vertices[j].y - vertices[i].y);
                newVertices.emplace_back(xIntersect, yCut);
            }

            if (vertices[i].y >= yCut) {
                newVertices.emplace_back(vertices[i].x, vertices[i].y);
            }

            if (vertices[j].y >= yCut && vertices[i].y < yCut) {
                // Intersection with horizontal line
                double xIntersect = vertices[i].x + (yCut - vertices[i].y) * (vertices[j].x - vertices[i].x) / (vertices[j].y - vertices[i].y);
                newVertices.emplace_back(xIntersect, yCut);
            }
        }

        vertices = std::move(newVertices);
        ensureCounterClockwise();
    }

    void printVertices() const {
        for (const auto& vertex : vertices) {
            std::cout << "(" << vertex.x << ", " << vertex.y << ")" << std::endl;
        }
    }

    std::vector<Point> getVertices() const {
        return vertices;
    }
};

class ReferencePolygon {
private:
    std::vector<Point> vertices;

public:
    void updateVertices(const std::vector<Point>& newVertices) {
        vertices = newVertices;
        ensureCounterClockwise();
    }

    void ensureCounterClockwise() {
        if (isClockwise()) {
            std::reverse(vertices.begin(), vertices.end());
        }
    }

    bool isClockwise() const {
        double sum = 0.0;
        int n = vertices.size();
        for (int i = 0; i < n; ++i) {
            int j = (i + 1) % n;
            sum += (vertices[j].x - vertices[i].x) * (vertices[j].y + vertices[i].y);
        }
        return sum > 0;
    }

    void printVertices() const {
        for (const auto& vertex : vertices) {
            std::cout << "(" << vertex.x << ", " << vertex.y << ")" << std::endl;
        }
    }
};

int main() {
    Polygon initialPolygon;
    
    int numVertices;
    std::cout << "Digite o número de vértices do polígono inicial: ";
    std::cin >> numVertices;

    if (numVertices < 3) {
        std::cerr << "O número de vértices deve ser pelo menos 3." << std::endl;
        return 1;
    }

    std::cout << "Digite as coordenadas dos vértices do polígono inicial:" << std::endl;
    for (int i = 0; i < numVertices; ++i) {
        double x, y;
        std::cout << "Vértice " << i + 1 << " (x y): ";
        std::cin >> x >> y;
        initialPolygon.addVertex(x, y);
    }

    std::cout << "Polígono inicial:" << std::endl;
    initialPolygon.printVertices();

    ReferencePolygon refPolygon;

    int numCuts;
    std::cout << "Digite o número de coordenadas de corte (horizontais): ";
    std::cin >> numCuts;

    if (numCuts < 1) {
        std::cerr << "O número de cortes deve ser pelo menos 1." << std::endl;
        return 1;
    }

    std::vector<double> cutCoordinates(numCuts);
    std::cout << "Digite as coordenadas de corte (uma por linha):" << std::endl;
    for (int i = 0; i < numCuts; ++i) {
        std::cout << "Corte " << i + 1 << ": ";
        std::cin >> cutCoordinates[i];
    }

    for (double cut : cutCoordinates) {
        initialPolygon.cutByHorizontalLine(cut);
    }

    refPolygon.updateVertices(initialPolygon.getVertices());

    std::cout << "Polígono de referência:" << std::endl;
    refPolygon.printVertices();

    return 0;
}