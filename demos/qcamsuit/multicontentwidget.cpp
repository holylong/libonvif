#include "multicontentwidget.h"
#include "imagerenderer.h"

MultiContentWidget::MultiContentWidget(QWidget *parent)
    : QWidget{parent}{
    setLayout(new QGridLayout(this));

    // 假设有48个已经初始化好的QLabel对象存储在一个容器中
    // _cameraViews.resize(4); // 初始化48个QLabel
    _renderers.resize(4);

    // 将监控画面添加到布局中
    int index = 0;
    auto rc = getOptimalDivision(4);
    _numCols = rc.first;
    _numRows = rc.second;
    qDebug() << _numCols << " " << _numRows;

    for (int row = 0; row < _numCols; ++row) {
        for (int col = 0; col < _numRows; ++col) {
            if (index < _renderers.size()) {
                // view = new QLabel(QString("监控画面 %1").arg(index));
                _renderers[index] = new TextureWidget();
                // view->setText(QString("monitor %1").arg(index));
                ((QGridLayout*)layout())->addWidget(_renderers[index], row, col);

                // QObject::connect(this, &MonitorGridWidget::resized, this, [=](QResizeEvent *event) {
                //     adjustViewSize(view);
                // });

                index++;
            }
        }
    }

    // 设置布局对齐方式，使各控件均匀分布
    layout()->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout()->setSpacing(0); // 取消间距以最大化利用空间
    layout()->setContentsMargins(0, 0, 0, 0); // 取消边距

    ignoreResizeEvent = false;
}

// 函数定义
std::pair<int, int> MultiContentWidget::getOptimalDivision(int n) {
    int best_r = 1;
    int best_c = n;
    double best_ratio_diff = std::abs((16.0 / 9) - (static_cast<double>(best_c) / best_r));

    for (int r = 1; r <= n; ++r) {
        int c = static_cast<int>(std::ceil(static_cast<double>(n) / r));
        if (r * c >= n) {
            double ratio_diff = std::abs((16.0 / 9) - (static_cast<double>(c) / r));
            if (ratio_diff < best_ratio_diff) {
                best_ratio_diff = ratio_diff;
                best_r = r;
                best_c = c;
            } else if (ratio_diff == best_ratio_diff && r * c == n) {
                // 如果比例差相同，优先选择正好能分成 n 个子矩形的分割方式
                best_r = r;
                best_c = c;
            }
        }
    }

    return {best_r, best_c};
}

void MultiContentWidget::setGridSize(int rows, int cols){
    // _layout->setRowCount(rows);
    // _layout->setColumnCount(cols);

    updateLayout();
}

void MultiContentWidget::setUrls(const QVector<QString>& urls){
    _urls = urls;


    updateLayout();
}

void MultiContentWidget::updateLayout(){
    qDebug() << __FUNCTION__ << " "  << _renderers.size() << " " << _urls.size();
    clearLayout();
    for(int i = 0; i < _renderers.size(); i++){

        if(i < _urls.size()){
            if(_renderers.at(0) == nullptr){
                qDebug() << "render is null";
            }
            qDebug() << "has url:" << _renderers.at(i)->getUrl();
            if(_renderers.at(i)->getUrl() == _urls.at(i)){
                qDebug() << "already open";
                continue;
            }

            _renderers.at(i)->setUrl(_urls.at(i));
        }
    }
}

void MultiContentWidget::clearLayout(){

}
