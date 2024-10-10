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
    
    std::vector<Point> vertices;

    void SP_Corte(int i, int j, float VLN, std::vector<Point>& newVertices) {
        double DELTAW = vertices[j].y - vertices[i].y;
        double DELTAU = vertices[j].x - vertices[i].x;

        if (std::abs(DELTAU) < 0.01) {
            return;
        }

        double COEFANG = DELTAW / DELTAU;
        double xIntersect = vertices[i].x - (vertices[i].y - VLN) / COEFANG;
        newVertices.emplace_back(xIntersect, VLN);
    }

    std::vector<Point> newVertices;

    std::vector<Point> cutByHorizontalLine(float VLN) {
        newVertices.clear();
        int NV = vertices.size();

        for (int i = 0; i < NV; i++) {
            int j = (i + 1) % NV;


            if(vertices[i].y <= VLN && vertices[j].y >= VLN){
                SP_Corte(i, j, VLN, newVertices);
            }

            if (vertices[i].y >= VLN && vertices[j].y <= VLN) {
                SP_Corte(i, j, VLN, newVertices);
            }

            if (vertices[i].y >= VLN && vertices[j].y >= VLN) {
                newVertices.emplace_back(vertices[i].x, vertices[i].y);
            }
        }

        return newVertices;
        // ensureCounterClockwise();
    }