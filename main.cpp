#include <iostream>
#include <fstream>

using namespace std;

int path_count = 0; // количество найденных путей


void detour(int v, int target, bool** g, int* path, int path_len,  int n, int* kol, bool *mark) {

    mark[v] = true; //вершина - посещенная

    path[path_len++] = v; //добавили вершину в текущий путь

    if (v == target) { //если вершина - та, до которой ищем путь
        for (int i = 0; i < path_len; i++) { //увеличиваем на 1 количество посещений вершин, принадлежащих найденному пути
            kol[path[i]]++;
        }
        path_count++; //увеличиваем количество найденных путей
    }
    else {
        for (int u = 0; u < n; u++) { //если есть ребро между данной вершимной и какой-либо еще запускаем функцию от смежной данной вершине
            if (g[v][u] && !mark[u]) {
                detour(u, target, g, path, path_len, n, kol,mark);
            }
        }
    }

    path_len--; //уменьшаем длину текущего пути

    mark[v] = false; //обозначаем вершину непосещённой
}

bool** create_adj(int kol_versh) { //создание матрицы смежности

    bool** g = new bool* [kol_versh];

    for (int i = 0; i < kol_versh; i++) {
        g[i] = new bool[kol_versh];
    }
    for (int i = 0; i < kol_versh; i++) { //заполняем созданную матрицу значениями false(0)
        for (int j = 0; j < kol_versh; j++) {
            g[i][j] = 0;
        }
    };
    return  g; // возвращаем матрицу смежности

}


int main() {

    setlocale(LC_ALL, "Ru");

    int sym1, sym2, kol_versh; 
    int start, target; // объявляем начальную и конечную вершины пути
    bool flag=true;

    ifstream graf; 
    ofstream out("result.txt");
    ifstream need_versh;

    need_versh.open("need.txt");//открываем файл на чтение

    graf.open("graf.txt");//открываем файл на чтение
    
    if (graf && need_versh) { //если файлы на чтение открыты

        graf >> kol_versh; //считываем количество вершин из файла graf.txt
        need_versh >> start; //считываем первую выделенную вершину из файла need.txt
        need_versh >> target; // считываем вторую выделенную вершину из файла need.txt

        if (start > 0 && target > 0) { //если файл need.txt не пустой и указанные в нём вершины не 0
            if (start != target) { //если "выделенные" вершины неодинаковы
                if (kol_versh >= 0) { //если файл не пустой
                    if (start <= kol_versh && target <= kol_versh) {  //если "выделенные" вершины принадлежат данному графу

                        bool** adj; //объявляем матрицу смежности

                        adj = create_adj(kol_versh); //создаем матрицу смежности и заполняем её значениями false по умолчанию

                        while (!graf.eof()) { //пока не считали все данные из файла заполняем матрицу смежности
                            graf >> sym1;
                            graf >> sym2;
                            adj[sym1 - 1][sym2 - 1] = 1;
                        }

                        if (adj[start - 1][target - 1]) { //если есть ребро между начальной и конечной вершиной - сразу выводим, что вершин нет
                            out << "Нет вершины, принадлежащей всем путям из " << start << " в " << target;
                        }
                        else {

                            int* kol = new int[kol_versh]; //сколько раз посещена вершина
                            for (int j = 0; j < kol_versh; j++) {
                                kol[j] = 0;
                            }
                            bool* mark = new bool[kol_versh]; //массив посещений вершины
                            for (int j = 0; j < kol_versh; j++) {
                                mark[j] = false;
                            }

                            int* path = new int[kol_versh]; // массив для хранения текущего пути

                            detour(start - 1, target - 1, adj, path, 0, kol_versh, kol, mark); //запускаем функцию поска всех путей и подсчёта вхождений вершин в пути

                            if (path_count == 0) { //если не найдено путей между "выделенными" вершинами
                                out << "Нет путей из " << start << " в " << target << endl;
                                flag = false;
                            }
                            else {
                                for (int j = 0; j < kol_versh; j++) {
                                    if (kol[j] == path_count && (j != start - 1) && (j != target - 1)) { //если если вершина принадлежит всем путям и не является "выделенной"
                                        out << "Вершина, входящая во все пути: " << j + 1 << endl;
                                        flag = false;
                                    }
                                }
                            }
                            if (flag) { //если нет вершин, принадлежащих всем путям 
                                out << "Нет вершины, принадлежащей всем путям из " << start << " в " << target;
                            }
                        }
                    }
                    else {
                        out << "Данная(ые) вершина(ы) не принадлежит(ат) заданному графу";
                    }
                }
                else {
                    out << "Файл 'graf.txt' пуст";
                    
                }
            }
                else {
                    out << "В файле 'need.txt' введены одинаковые вершины";
                }
        }
        else {
            out << "Файл 'need.txt' пуст или введена неподходящая условию задачи вершина 0 ";
         }
    }
    else {
        out << "Ошибка при открытии файла";
    }
    return 0;
}
