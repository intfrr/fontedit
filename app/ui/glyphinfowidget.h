#ifndef GLYPHINFOWIDGET_H
#define GLYPHINFOWIDGET_H

#include <QGraphicsWidget>
#include <QImage>
#include <QAction>
#include <f2b.h>

#include <optional>

class GlyphInfoWidget : public QGraphicsWidget
{
    Q_OBJECT

public:
    static constexpr auto cellMargin = 6.0;
    static constexpr auto descriptionHeight = 50.0;

    GlyphInfoWidget(const f2b::font::glyph& glyph, std::size_t index, bool isExported,
                    unsigned char asciiCode, QSizeF imageSize,
                    f2b::font::margins margins = {}, QGraphicsItem *parent = nullptr);

    std::size_t glyphIndex() const { return glyphIndex_; }
    void updateGlyph(std::optional<f2b::font::glyph> glyph, std::optional<bool> isExported = {}, std::optional<f2b::font::margins> margins = {});

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void setIsExportedAdjustable(bool isEnabled);

signals:
    void isExportedChanged(bool isExported);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private:
    const QString description_;
    const QSizeF imageSize_;
    bool isExportedAdjustable_;
    bool isExported_;
    QImage preview_;
    f2b::font::margins margins_;
    QAction toggleExportedAction_;

    std::size_t glyphIndex_;
};

#endif // GLYPHINFOWIDGET_H
