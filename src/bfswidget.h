    #ifndef BFSWIDGET_H
    #define BFSWIDGET_H
    #include"graph.h"
    #include <QDialog>
    #include"visualizegraph.h"
    namespace Ui {
    class BFSwidget;
    }

    class BFSwidget : public QDialog
    {
        Q_OBJECT

    public:
        explicit BFSwidget(Graph *graph, visualizeGraph *graphWid ,QWidget *parent = nullptr);
        ~BFSwidget();
    private slots:
        void on_pushButton_clicked();

    private:
        Ui::BFSwidget *ui;
        Graph *graph;
        visualizeGraph *graphWid;
        Node *startNode;
    };

    #endif // BFSWIDGET_H
