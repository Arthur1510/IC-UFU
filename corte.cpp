#include <iostream>
#include <vector>
#include <cmath>

struct Point {
    double x;
    double y;

    Point(double x_val, double y_val) : x(x_val), y(y_val) {}
};

// Função para verificar onde o corte deve ocorrer
int verificarCaso(const Point& p1, const Point& p2, double cortar) {
    if ((p2.y <= cortar && p1.y >= cortar) || (p2.y >= cortar && p1.y <= cortar)) {
        return 2; // Caso 2: Corta o trecho
    }
    return 1; // Caso 1: Adiciona o nó final na nova poligonal
}

// Função para calcular o ponto de interseção
Point calcularIntersecao(const Point& p1, const Point& p2, double cortar) {
    double deltaY = p2.y - p1.y;
    double deltaX = p2.x - p1.x;
    
    if (std::abs(deltaX) < 1e-6) {
        return Point(p1.x, cortar);
    } else {
        double tg = deltaY / deltaX;
        double x = p1.x - (p1.y - cortar) / tg;
        return Point(x, cortar);
    }
}

std::vector<Point> cortarPoligonal(const std::vector<Point>& secao, const std::vector<double>& cortar) {
    std::vector<Point> resultado = secao;

    for (double nivel : cortar) {
        std::vector<Point> novaPoligonal;
        int nv = resultado.size();

        for (int i = 0; i < nv; i++) {
            int caso = verificarCaso(resultado[i], resultado[(i + 1) % nv], nivel);

            if (caso == 2) {
                Point intersecao = calcularIntersecao(resultado[i], resultado[(i + 1) % nv], nivel);
                novaPoligonal.push_back(intersecao);
            }

            novaPoligonal.push_back(resultado[(i + 1) % nv]);
        }

        resultado = novaPoligonal;
    }

    return resultado;
}

// Função principal para teste
int main() {
    std::vector<Point> secao = {
        {0, 0}, {1, 0}, {1, 1}, {0, 1}
    };
    std::vector<double> cortar = {0.5, 0.7};

    std::vector<Point> resultado = cortarPoligonal(secao, cortar);

    std::cout << "Pontos após o corte:" << std::endl;
    for (const auto& p : resultado) {
        std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
    }

    return 0;
}