/*  This file is part of the KDE project
    Copyright (C) 2009 Colin Guthrie <cguthrie@mandriva.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), Nokia Corporation
    (or its successors, if any) and the KDE Free Qt Foundation, which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef PHONON_PULSESUPPORT_H
#define PHONON_PULSESUPPORT_H

#include "phonon_export.h"
#include "phononnamespace.h"
#include "objectdescription.h"

#include <QtCore/QtGlobal>
#include <QtCore/QSet>


namespace Phonon
{
    class PulseStream;
    class PHONON_EXPORT PulseSupport : public QObject
    {
        Q_OBJECT
        public:
            static PulseSupport* getInstance();
            static void shutdown();

            bool isActive();
            void enable(bool enabled = true);

            QList<int> objectDescriptionIndexes(ObjectDescriptionType type) const;
            QHash<QByteArray, QVariant> objectDescriptionProperties(ObjectDescriptionType type, int index) const;
            QList<int> objectIndexesByCategory(ObjectDescriptionType type, Category category) const;
            QList<int> objectIndexesByCategory(ObjectDescriptionType type, CaptureCategory category) const;

            void setOutputDevicePriorityForCategory(Category category, QList<int> order);
            void setCaptureDevicePriorityForCategory(CaptureCategory category, QList<int> order);

            PHONON_DEPRECATED void setCaptureDevicePriorityForCategory(Category category, QList<int> order);

            PulseStream *registerOutputStream(QString streamUuid, Category category);
            PulseStream *registerCaptureStream(QString streamUuid, CaptureCategory category);
            PHONON_DEPRECATED PulseStream *registerCaptureStream(QString streamUuid, Category category);

            /**
             * Whenever possible this function should be used to get Phonon
             * specific PulseAudio stream properties and set them on specific
             * streams. When precisely setting them per stream is not possible
             * the envrionment setup function PulseSupport::setupStreamEnvironment
             * should be called as close to stream creation as possible. The
             * more time passes between setup and stream creation the more
             * likely race conditions between setup of more than one AudioOutput
             * will appear.
             *
             * \param streamUuid the AudioOutputs' stream UUID set by the frontend through
             *                   AudioOutputInterface47::setStreamUuid
             *
             * \returns a hash of all properties set by setupStreamEnvironment
             *
             * \see setupStreamEnvironment
             * \since 4.7.0
             */
            QHash<QString, QString> streamProperties(QString streamUuid) const;

            /**
             * Sets PulseAudio override properties in the process' envrionment.
             * Manually setting the properties on a per-stream basis is
             * preferred as envrionment overrides are subject to race conditions
             * when creating more than one stream around the same time.
             *
             * \param streamUuid the AudioOutputs' stream UUID set by the frontend
             *                   through AudioOutputInterface47::setStreamUuid
             *
             * \see streamProperties
             * \since 4.7.0
             */
            void setupStreamEnvironment(QString streamUuid);

            void emitObjectDescriptionChanged(ObjectDescriptionType);

            bool setOutputName(QString streamUuid, QString name);
            bool setOutputDevice(QString streamUuid, int device);
            bool setOutputVolume(QString streamUuid, qreal volume);
            bool setOutputMute(QString streamUuid, bool mute);
            bool setCaptureDevice(QString streamUuid, int device);
            // NB Capture Volume/Mute not set until PA supports per-source-output volumes/mutes
            //    or phonon supports capture properly... which ever comes first.
            void clearStreamCache(QString streamUuid);

            static void debug();
        public Q_SLOTS:
            void connectToDaemon();

        Q_SIGNALS:
            void objectDescriptionChanged(ObjectDescriptionType);

        private:
            PulseSupport();
            ~PulseSupport();

            bool mEnabled;
    };
} // namespace Phonon


#endif // PHONON_PULSESUPPORT_H
