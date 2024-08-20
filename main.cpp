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
    std::vector<Point> translatedVertices;

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

    double staticMomentX() const {
        double Sx = 0.0;
        int n = vertices.size();
        for (int i = 0; i < n; ++i) {
            int j = (i + 1) % n;
            Sx += (vertices[i].y + vertices[j].y) * (vertices[i].x * vertices[j].y - vertices[j].x * vertices[i].y);
        }
        return std::abs(Sx) / 2.0;
    }

    double staticMomentY() const {
        double Sy = 0.0;
        int n = vertices.size();
        for (int i = 0; i < n; ++i) {
            int j = (i + 1) % n;
            Sy += (vertices[i].x + vertices[j].x) * (vertices[i].x * vertices[j].y - vertices[j].x * vertices[i].y);
        }
        return std::abs(Sy) / 2.0;
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

    void rotate(double angle) {
        Point cg = centroid();
        double radians = angle * M_PI / 180.0;
        int n = translatedVertices.size();

        // Percorre cada vértice em translatedVertices usando índice
        for (size_t i = 0; i < n; ++i) {
            double x_new = (translatedVertices[i].x - cg.x) * cos(radians) - (translatedVertices[i].y - cg.y) * sin(radians) + cg.x;
            double y_new = (translatedVertices[i].x - cg.x) * sin(radians) + (translatedVertices[i].y - cg.y) * cos(radians) + cg.y;

            translatedVertices[i].x = x_new;
            translatedVertices[i].y = y_new;
        }
    }

    void getYRange(double &yMin, double &yMax) const {
        if (translatedVertices.empty()) return;

        yMin = translatedVertices[0].y;
        yMax = translatedVertices[0].y;

        int n = translatedVertices.size();

        // Percorre os vértices em translatedVertices usando índice
        for (size_t i = 1; i < n; ++i) {
            if (translatedVertices[i].y < yMin) yMin = translatedVertices[i].y;
            if (translatedVertices[i].y > yMax) yMax = translatedVertices[i].y;
        }
    }

};

int main() {
    Polygon polygon;
    int numVertices;

    std::cout << "Insira o número de vértices do polígono: ";
    std::cin >> numVertices;

    for (int i = 0; i < numVertices; ++i) {
        double x, y;
        std::cout << "Insira as coordenadas do vértice " << i + 1 << " (formato: x y): ";
        std::cin >> x >> y;
        polygon.addVertex(x, y);
    }

    // Garantir que os vértices estão em ordem anti-horária
    polygon.ensureCounterClockwise();

    // Cálculo da área
    double area = polygon.area();
    std::cout << "Área: " << area << std::endl;

    // Cálculo do centro de gravidade
    Point cg = polygon.centroid();
    std::cout << "Centro de Gravidade (CG): (" << cg.x << ", " << cg.y << ")" << std::endl;

    // Cálculo dos momentos estáticos
    double Sx = polygon.staticMomentX();
    double Sy = polygon.staticMomentY();
    std::cout << "Momento Estático em relação ao eixo x (Sx): " << Sx << std::endl;
    std::cout << "Momento Estático em relação ao eixo y (Sy): " << Sy << std::endl;

    // Translada os vértices para que o centróide coincida com a origem
    std::vector<Point> translatedVertices = polygon.translateToCG();
    std::cout << "Vértices transladados para o centróide:" << std::endl;
    for (const auto& vertex : translatedVertices) {
        std::cout << "(" << vertex.x << ", " << vertex.y << ")" << std::endl;
    }

    // Rotaciona o polígono em 45 graus
    double angle = 0.0;
    polygon.rotate(angle);
    std::cout << "Polígono rotacionado em " << angle << " graus." << std::endl;

    // Obtém e exibe o intervalo de Y após a rotação
    double yMin, yMax;
    polygon.getYRange(yMin, yMax);
    std::cout << "Intervalo de Y após a rotação: [" << yMin << ", " << yMax << "]" << std::endl;

    return 0;

}