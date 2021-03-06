#include "f2b_qt_compat.h"

static constexpr quint32 font_glyph_magic_number = 0x92588c12;
static constexpr quint32 font_face_magic_number = 0x03f59a82;

static constexpr quint32 font_glyph_version = 1;
static constexpr quint32 font_face_version = 2;

using namespace f2b;

QDataStream& operator<<(QDataStream& s, const font::glyph& glyph)
{
    s << font_glyph_magic_number;
    s << font_glyph_version;
    s.setVersion(QDataStream::Qt_5_7);
    s << (quint32) glyph.size().width;
    s << (quint32) glyph.size().height;
    s << glyph.pixels();

    return s;
}

QDataStream& operator>>(QDataStream& s, font::glyph& glyph)
{
    quint32 magic_number;
    quint32 version;
    s >> magic_number >> version;
    if (magic_number == font_glyph_magic_number && version == font_glyph_version) {
        s.setVersion(QDataStream::Qt_5_7);
        quint32 width, height;
        s >> width >> height;

        std::vector<bool> pixels;
        pixels.reserve(width * height);
        s >> pixels;

        glyph = font::glyph({width, height}, pixels);
    }

    return s;
}

QDataStream& operator<<(QDataStream& s, const font::face& face)
{
    s << font_face_magic_number;
    s << font_face_version;
    s.setVersion(QDataStream::Qt_5_7);
    s << (quint32) face.glyphs_size().width;
    s << (quint32) face.glyphs_size().height;
    s << face.glyphs();
    s << face.exported_glyph_ids();

    return s;

}

QDataStream& operator>>(QDataStream& s, font::face& face)
{
    quint32 magic_number;
    quint32 version;
    s >> magic_number >> version;
    if (magic_number == font_face_magic_number) {
        s.setVersion(QDataStream::Qt_5_7);
        quint32 width, height;
        s >> width >> height;

        std::vector<font::glyph> glyphs;
        s >> glyphs;

        std::set<std::size_t> exported_glyph_ids;
        if (version < 2) {
            for (std::size_t i = 0; i < glyphs.size(); i++) {
                exported_glyph_ids.insert(i);
            }
        } else {
            s >> exported_glyph_ids;
        }
        face = font::face({width, height}, glyphs, exported_glyph_ids);
    }

    return s;
}

QVariant to_qvariant(const source_code::indentation& i) {
    if (std::holds_alternative<source_code::tab>(i)) {
        return QVariant(-1);
    } else if (std::holds_alternative<source_code::space>(i)) {
        return QVariant((uint)std::get<source_code::space>(i).num_spaces);
    }
    return QVariant();
}

source_code::indentation from_qvariant(const QVariant& v) {
    bool ok;
    auto intValue = v.toInt(&ok);
    if (ok && intValue == -1) {
        return source_code::tab {};
    }
    auto uintValue = v.toUInt(&ok);
    if (ok) {
        return source_code::space { uintValue };
    }
    return source_code::tab {};
}
