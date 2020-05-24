#ifndef SOURCECODERUNNABLE_H
#define SOURCECODERUNNABLE_H

#include <QRunnable>
#include <QString>
#include <f2b.h>
#include <memory>
#include <functional>
#include <optional>
#include <mutex>

class SourceCodeRunnable : public QRunnable
{
    using CompletionHandler = std::function<void(const QString&)>;

public:
    SourceCodeRunnable(f2b::Font::Face face, f2b::SourceCodeOptions options,
                       const QString& format, const QString& fontArrayName)
        : QRunnable(),
          face_ { std::move(face) },
          generator_ { options },
          format_ { format.toStdString() },
          fontArrayName_ { fontArrayName.toStdString() }
    {};

    void run() override;

    void setCompletionHandler(CompletionHandler handler) {
        handler_ = std::move(handler);
    }

    bool isFinished();

    bool isCanceled();
    void setCanceled(bool canceled);

protected:
    void setFinished(bool finished);

private:
    bool m_finished { false };
    bool m_canceled { false };
    std::mutex mutex_;

    f2b::Font::Face face_;
    f2b::FontSourceCodeGenerator generator_;
    std::string format_;
    std::string fontArrayName_;
    CompletionHandler handler_ {};
};

#endif // SOURCECODERUNNABLE_H
