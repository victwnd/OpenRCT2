/*****************************************************************************
 * Copyright (c) 2014-2024 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "../../../drawing/Drawing.h"
#include "../../../interface/Viewport.h"
#include "../../../ride/RideData.h"
#include "../../../ride/TrackData.h"
#include "../../../ride/TrackPaint.h"
#include "../../../sprites.h"
#include "../../../world/Map.h"
#include "../../Paint.h"
#include "../../support/MetalSupports.h"
#include "../../tile_element/Segment.h"
#include "../../track/Segment.h"
#include "../../track/Support.h"

using namespace OpenRCT2;

namespace OpenRCT2::SingleRailRC
{
    static void TrackFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_FLAT + 0)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_FLAT + 1)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_FLAT + 2)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_FLAT + 3)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT + 0)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT + 1)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
            }
        }
        PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
    }

    static void TrackStation(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        static constexpr uint32_t imageIds[4][3] = {
            { (SPR_G2_SINGLE_RAIL_TRACK_BRAKE + 0), (SPR_G2_SINGLE_RAIL_TRACK_BLOCK_BRAKE + 0), SPR_STATION_BASE_B_SW_NE },
            { (SPR_G2_SINGLE_RAIL_TRACK_BRAKE + 1), (SPR_G2_SINGLE_RAIL_TRACK_BLOCK_BRAKE + 1), SPR_STATION_BASE_B_NW_SE },
            { (SPR_G2_SINGLE_RAIL_TRACK_BRAKE + 0), (SPR_G2_SINGLE_RAIL_TRACK_BLOCK_BRAKE + 0), SPR_STATION_BASE_B_SW_NE },
            { (SPR_G2_SINGLE_RAIL_TRACK_BRAKE + 1), (SPR_G2_SINGLE_RAIL_TRACK_BLOCK_BRAKE + 1), SPR_STATION_BASE_B_NW_SE },
        };

        if (trackElement.GetTrackType() == TrackElemType::EndStation)
        {
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours.WithIndex(imageIds[direction][1]), { 0, 0, height },
                { { 0, 6, height + 3 }, { 32, 20, 1 } });
        }
        else
        {
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours.WithIndex(imageIds[direction][0]), { 0, 0, height },
                { { 0, 6, height + 3 }, { 32, 20, 1 } });
        }
        PaintAddImageAsParentRotated(
            session, direction, GetStationColourScheme(session, trackElement).WithIndex(imageIds[direction][2]),
            { 0, 0, height - 2 }, { { 0, 2, height }, { 32, 28, 2 } });
        DrawSupportsSideBySide(session, direction, height, session.SupportColours, supportType.metal);
        TrackPaintUtilDrawStation2(session, ride, direction, height, trackElement, 4, 7);
        TrackPaintUtilDrawStationTunnel(session, direction, height);
        PaintUtilSetSegmentSupportHeight(session, kSegmentsAll, 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
    }

    static void Track25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE + 8)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE + 9)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE + 10)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE + 11)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE + 8)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE + 9)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE + 10)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE + 11)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
            }
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void Track60DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP + 12)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP + 13)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 98 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP + 14)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 98 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP + 15)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 32, height, session.SupportColours);
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP + 12)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP + 13)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 98 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP + 14)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 98 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP + 15)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 32, height, session.SupportColours);
            }
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 56, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 104);
    }

    static void TrackFlatTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE + 0)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE + 1)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE + 2)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE + 3)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE + 0)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE + 1)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE + 2)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE + 3)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
            }
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 48);
    }

    static void Track25DegUpTo60DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP + 0)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP + 1)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP + 2)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 66 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP + 3)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP + 4)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 66 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP + 5)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 12, height, session.SupportColours);
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP + 0)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP + 1)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP + 2)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 66 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP + 3)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP + 4)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 66 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP + 5)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 12, height, session.SupportColours);
            }
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 24, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 72);
    }

    static void Track60DegUpTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP + 6)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP + 7)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP + 8)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 66 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP + 9)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP + 10)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 66 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP + 11)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 20, height, session.SupportColours);
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP + 6)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP + 7)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP + 8)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 66 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP + 9)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP + 10)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 66 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP + 11)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 20, height, session.SupportColours);
            }
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 24, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 72);
    }

    static void Track25DegUpToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE + 4)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE + 5)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE + 6)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE + 7)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE + 4)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE + 5)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE + 6)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE + 7)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
            }
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::FlatTo25Deg);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 40);
    }

    static void Track25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        Track25DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void Track60DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        Track60DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackFlatTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        Track25DegUpToFlat(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void Track25DegDownTo60DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        Track60DegUpTo25DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void Track60DegDownTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        Track25DegUpTo60DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void Track25DegDownToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackFlatTo25DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void Track90DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL + 8)),
                            { 0, 0, height }, { { 4, 6, height + 8 }, { 2, 20, 31 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL + 9)),
                            { 0, 0, height }, { { 24, 6, height + 8 }, { 2, 20, 31 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL + 10)),
                            { 0, 0, height }, { { 24, 6, height + 8 }, { 2, 20, 31 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL + 11)),
                            { 0, 0, height }, { { 4, 6, height + 8 }, { 2, 20, 31 } });
                        break;
                }
                PaintUtilSetVerticalTunnel(session, height + 32);
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                break;
        }
    }

    static void Track90DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        Track90DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void Track60DegUpTo90DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL + 1)),
                            { 0, 0, height }, { { 24, 6, height }, { 2, 20, 55 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL + 2)),
                            { 0, 0, height }, { { 24, 6, height }, { 2, 20, 55 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL + 3)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
                {
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 36, height, session.SupportColours);
                }
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetVerticalTunnel(session, height + 56);
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                break;
        }
    }

    static void Track90DegDownTo60DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        Track60DegUpTo90DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void Track90DegUpTo60DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL + 4)),
                    { 0, 0, height }, { { 0, 6, height + 8 }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL + 5)),
                    { 0, 0, height }, { { 24, 6, height + 8 }, { 2, 20, 31 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL + 6)),
                    { 0, 0, height }, { { 24, 6, height + 8 }, { 2, 20, 31 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL + 7)),
                    { 0, 0, height }, { { 0, 6, height + 8 }, { 32, 20, 3 } });
                break;
        }
        switch (direction)
        {
            case 1:
                PaintUtilPushTunnelRight(session, height + 48, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                break;
            case 2:
                PaintUtilPushTunnelLeft(session, height + 48, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                break;
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 80);
    }

    static void Track60DegDownTo90DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL + 6)),
                            { 0, 0, height }, { { 24, 6, height + 8 }, { 2, 20, 31 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL + 7)),
                            { 0, 0, height }, { { 0, 6, height + 8 }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL + 4)),
                            { 0, 0, height }, { { 0, 6, height + 8 }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL + 5)),
                            { 0, 0, height }, { { 24, 6, height + 8 }, { 2, 20, 31 } });
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height + 48, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 80);
                break;
            case 1:
                break;
        }
    }

    static void TrackLeftQuarterTurn3(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE + 3)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE + 6)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE + 9)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE + 1)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE + 4)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE + 7)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE + 10)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE + 2)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE + 5)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE + 8)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE + 11)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackRightQuarterTurn3(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
        TrackLeftQuarterTurn3(session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void TrackLeftQuarterTurn5(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 5)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 10)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 15)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 1)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 6)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 11)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 16)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 2)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 7)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 12)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 17)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 3)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 8)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 13)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 18)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 4)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 9)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 14)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE + 19)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackRightQuarterTurn5(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
        TrackLeftQuarterTurn5(session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void TrackLeftEighthToDiag(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 4)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 8)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 12)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 1)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 5)),
                            { 0, 0, height }, { { 0, 0, height }, { 34, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 9)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 13)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 2)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 6)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 10)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 14)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 3)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 7)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 18, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 11)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 15)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackRightEighthToDiag(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 16)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 20)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 24)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 28)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 17)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 21)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 25)),
                            { 0, 0, height }, { { 0, 0, height }, { 34, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 29)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 18)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 22)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 26)),
                            { 0, 0, height }, { { 4, 4, height }, { 28, 28, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 30)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 19)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 23)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 27)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 18, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE + 31)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackLeftEighthToOrthogonal(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
        TrackRightEighthToDiag(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackRightEighthToOrthogonal(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
        TrackLeftEighthToDiag(session, ride, trackSequence, (direction + 3) & 3, height, trackElement, supportType);
    }

    static void TrackDiagFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        constexpr ImageIndex images[2][kNumOrthogonalDirections] = {
            { SPR_G2_SINGLE_RAIL_TRACK_FLAT_DIAGONAL + 0, SPR_G2_SINGLE_RAIL_TRACK_FLAT_DIAGONAL + 1,
              SPR_G2_SINGLE_RAIL_TRACK_FLAT_DIAGONAL + 0, SPR_G2_SINGLE_RAIL_TRACK_FLAT_DIAGONAL + 1 },
            { SPR_G2_SINGLE_RAIL_LIFT_TRACK_FLAT_DIAGONAL + 0, SPR_G2_SINGLE_RAIL_LIFT_TRACK_FLAT_DIAGONAL + 1,
              SPR_G2_SINGLE_RAIL_LIFT_TRACK_FLAT_DIAGONAL + 2, SPR_G2_SINGLE_RAIL_LIFT_TRACK_FLAT_DIAGONAL + 3 },
        };

        TrackPaintUtilDiagTilesPaintExtra(
            session, 3, height, direction, trackSequence, images[trackElement.HasChain()], supportType.metal);
    }

    static void TrackDiagBrakes(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BRAKE_DIAGONAL + 1)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BRAKE_DIAGONAL + 0)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BRAKE_DIAGONAL + 0)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BRAKE_DIAGONAL + 1)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackDiagBlockBrakes(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BLOCK_BRAKE_DIAGONAL + 1)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BLOCK_BRAKE_DIAGONAL + 0)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BLOCK_BRAKE_DIAGONAL + 0)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BLOCK_BRAKE_DIAGONAL + 1)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackDiag25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 11)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 11)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 8)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 8)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 10)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 10)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 9, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 9, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 9, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 9, height, session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 9, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 9, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 9, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 9, height, session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void TrackDiag25DegUpToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 7)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 7)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 4)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 4)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 6)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 6)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 6, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 5)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 6, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 6, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 6, height, session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 6, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 5)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 6, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 6, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 6, height, session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void TrackDiagFlatTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 3)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 3)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 0)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 0)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 2)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 2)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height + 2,
                                session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 0, height + 2,
                                session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 2, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 2, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 2, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 2, height, session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackDiag25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 9)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 9)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 10)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 10)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 8)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 8)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 11)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 11)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void TrackDiagFlatTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 5)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 5)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 6)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 6)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 4)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 4)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 7)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 7)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                break;
        }

        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void TrackDiag25DegDownToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 2)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 2)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 0)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 0)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_GENTLE_DIAGONAL + 3)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_DIAGONAL + 3)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackDiag60DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 11)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 11)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 104);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 8)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 8)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 104);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 10)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 10)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 104);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 38, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 9)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 36, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 38, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 36, height,
                                session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 38, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 9)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 36, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 38, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 36, height,
                                session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 104);
                break;
        }
    }

    static void TrackDiag25DegUpTo60DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 3)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 3)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 0)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 0)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 2)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 2)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 16, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 16, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 16, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 16, height,
                                session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 16, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 16, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 16, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 16, height,
                                session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackDiag60DegUpTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 7)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 7)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 4)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 4)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 6)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 6)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 21, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 5)),
                                { -16, -16, height }, { { 0, 0, height }, { 16, 16, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 21, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 21, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 21, height,
                                session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 21, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 5)),
                                { -16, -16, height }, { { 0, 0, height }, { 16, 16, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 21, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 21, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 21, height,
                                session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackDiag60DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 9)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 9)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 104);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 10)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 10)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 104);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 8)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 8)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 104);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 24, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 11)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 28, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 24, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 28, height,
                                session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 24, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 11)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 28, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 24, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 28, height,
                                session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 104);
                break;
        }
    }

    static void TrackDiag25DegDownTo60DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 5)),
                                { -16, -16, height }, { { 0, 0, height }, { 16, 16, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 5)),
                                { -16, -16, height }, { { 0, 0, height }, { 16, 16, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 6)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 6)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 4)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 4)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 17, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 7)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 17, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 17, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 17, height,
                                session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 17, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 7)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 17, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 17, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 17, height,
                                session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackDiag60DegDownTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 2)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 2)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 0)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 0)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_STEEP_DIAGONAL + 3)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_DIAGONAL + 3)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackFlatToLeftBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 0)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 1)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 2)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 3)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 4)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 5)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
        }
        PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
    }

    static void TrackFlatToRightBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 6)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 7)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 8)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 9)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 10)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 11)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
        }
        PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
    }

    static void TrackLeftBankToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackFlatToRightBank(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackRightBankToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackFlatToLeftBank(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackLeftBankTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 12)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 13)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 14)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 15)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 16)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 17)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 48);
    }

    static void TrackRightBankTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 18)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 19)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 20)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 21)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 22)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 23)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 48);
    }

    static void Track25DegUpToLeftBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 24)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 25)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 26)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 27)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 28)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 29)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::FlatTo25Deg);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 40);
    }

    static void Track25DegUpToRightBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 30)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 31)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 32)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 33)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 34)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 35)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::FlatTo25Deg);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 40);
    }

    static void TrackLeftBankTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        Track25DegUpToRightBank(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackRightBankTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        Track25DegUpToLeftBank(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void Track25DegDownToLeftBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackRightBankTo25DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void Track25DegDownToRightBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackLeftBankTo25DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackLeftBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 36)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 37)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 38)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION + 39)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
        }
        PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
    }

    static void TrackRightBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackLeftBank(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackDiagFlatToLeftBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 4)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 0)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 1)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 3)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 2)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackDiagFlatToRightBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 9)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 5)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 7)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 8)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 6)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackDiagLeftBankToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 6)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 7)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 8)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 5)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 9)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackDiagRightBankToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 2)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 3)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 0)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 1)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 4)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackDiagLeftBankTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 14)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 10)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 11)),
                            { -16, -16, height }, { { -16, -16, height + 35 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 13)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 12)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackDiagRightBankTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 19)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 15)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 17)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 18)),
                            { -16, -16, height }, { { -16, -16, height + 35 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 16)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackDiag25DegUpToLeftBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 24)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 20)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 21)),
                            { -16, -16, height }, { { -16, -16, height + 35 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 23)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 22)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                        break;
                    case 2:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                        break;
                    case 3:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void TrackDiag25DegUpToRightBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 29)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 25)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 27)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 28)),
                            { -16, -16, height }, { { -16, -16, height + 35 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 26)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                        break;
                    case 2:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                        break;
                    case 3:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void TrackDiagLeftBankTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 26)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 27)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 28)),
                            { -16, -16, height }, { { -16, -16, height + 35 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 25)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 29)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                        break;
                    case 2:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                        break;
                    case 3:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                break;
        }

        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void TrackDiagRightBankTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 22)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 23)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 20)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 21)),
                            { -16, -16, height }, { { -16, -16, height + 35 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 24)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                        break;
                    case 2:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                        break;
                    case 3:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                break;
        }

        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void TrackDiag25DegDownToLeftBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 16)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 17)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 18)),
                            { -16, -16, height }, { { -16, -16, height + 35 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 15)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 19)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackDiag25DegDownToRightBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 12)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 13)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 10)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 11)),
                            { -16, -16, height }, { { -16, -16, height + 35 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 14)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackDiagLeftBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 33)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 30)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 32)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 31)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackDiagRightBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 31)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 32)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 30)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BANK_TRANSITION_DIAGONAL + 33)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackLeftQuarterTurn3Bank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE_BANKED + 0)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE_BANKED + 1)), { 0, 0, height },
                            { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE_BANKED + 4)), { 0, 0, height },
                            { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE_BANKED + 7)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE_BANKED + 11)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 2, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE_BANKED + 2)), { 0, 0, height },
                            { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE_BANKED + 5)), { 0, 0, height },
                            { { 0, 0, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE_BANKED + 8)), { 0, 0, height },
                            { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE_BANKED + 12)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE_BANKED + 3)), { 0, 0, height },
                            { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE_BANKED + 6)), { 0, 0, height },
                            { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE_BANKED + 9)), { 0, 0, height },
                            { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE_BANKED + 10)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_CURVE_BANKED + 13)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 2, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackRightQuarterTurn3Bank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
        TrackLeftQuarterTurn3Bank(session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void TrackBankedLeftQuarterTurn5(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 1)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 6)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 11)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 17)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 1, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 2)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 7)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 32, 16, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 12)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 18)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 3)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 8)),
                            { 0, 0, height }, { { 16, 16, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 13)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 19)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 4)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 9)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 16, 32, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 14)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 20)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 5)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 10)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 15)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 16)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_CURVE_BANKED + 21)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 1, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackBankedRightQuarterTurn5(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
        TrackBankedLeftQuarterTurn5(session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void TrackLeftEighthBankToDiag(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 0)), { 0, 0, height },
                            { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 4)), { 0, 0, height },
                            { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 8)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 12)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 1)), { 0, 0, height },
                            { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 5)), { 0, 0, height },
                            { { 0, 0, height + 27 }, { 34, 16, 0 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 9)), { 0, 0, height },
                            { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 13)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 2)), { 0, 0, height },
                            { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 6)), { 0, 0, height },
                            { { 16, 16, height + 27 }, { 16, 16, 0 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 10)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 14)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 3)), { 0, 0, height },
                            { { 16, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 7)), { 0, 0, height },
                            { { 0, 16, height + 27 }, { 16, 18, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 11)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 15)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackRightEighthBankToDiag(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 16)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 20)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 24)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 28)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 17)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 21)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 25)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 34, 16, 0 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 29)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 18)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 22)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 26)),
                            { 0, 0, height }, { { 4, 4, height + 27 }, { 28, 28, 0 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 30)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 19)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 23)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 27)),
                            { 0, 0, height }, { { 0, 16, height + 27 }, { 16, 18, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CURVE_BANKED + 31)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackLeftEighthBankToOrthogonal(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
        TrackRightEighthBankToDiag(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackRightEighthBankToOrthogonal(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
        TrackLeftEighthBankToDiag(session, ride, trackSequence, (direction + 3) & 3, height, trackElement, supportType);
    }

    static void TrackLeftQuarterTurn3Tile25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE + 0)), { 0, 6, height },
                            { 32, 20, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE + 2)), { 0, 6, height },
                            { 32, 20, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE + 4)), { 0, 6, height },
                            { 32, 20, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE + 6)), { 0, 6, height },
                            { 32, 20, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE + 1)), { 6, 0, height },
                            { 20, 32, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE + 3)), { 6, 0, height },
                            { 20, 32, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE + 5)), { 6, 0, height },
                            { 20, 32, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE + 7)), { 6, 0, height },
                            { 20, 32, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackRightQuarterTurn3Tile25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE + 8)), { 0, 6, height },
                            { 32, 20, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE + 10)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE + 12)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE + 14)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE + 9)), { 6, 0, height },
                            { 20, 32, 3 });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE + 11)),
                            { 6, 0, height }, { 20, 32, 3 });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE + 13)),
                            { 6, 0, height }, { 20, 32, 3 });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 10, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE + 15)),
                            { 6, 0, height }, { 20, 32, 3 });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                        break;
                }
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackLeftQuarterTurn3Tile25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
        TrackRightQuarterTurn3Tile25DegUp(session, ride, trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void TrackRightQuarterTurn3Tile25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
        TrackLeftQuarterTurn3Tile25DegUp(session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void TrackLeftQuarterTurn525DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 5)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 10)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 15)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 1)),
                            { 0, 0, height }, { 32, 16, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 6)),
                            { 0, 0, height }, { 32, 16, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 11)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 16)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 2)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 7)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 12)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 17)),
                            { 0, 0, height }, { 16, 16, 3 });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 4:
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 3)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 8)),
                            { 0, 0, height }, { 16, 32, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 13)),
                            { 0, 0, height }, { 16, 32, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 18)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 4)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 9)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 14)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 19)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackRightQuarterTurn525DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 20)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 25)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 30)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 35)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 21)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 26)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 31)),
                            { 0, 0, height }, { 32, 16, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 36)),
                            { 0, 0, height }, { 32, 16, 3 });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 22)),
                            { 0, 0, height }, { 16, 16, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 27)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 32)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 37)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 4:
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 23)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 28)),
                            { 0, 0, height }, { 16, 32, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 33)),
                            { 0, 0, height }, { 16, 32, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 38)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 24)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 29)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 34)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE + 39)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackLeftQuarterTurn525DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
        TrackRightQuarterTurn525DegUp(session, ride, trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void TrackRightQuarterTurn525DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
        TrackLeftQuarterTurn525DegUp(session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void TrackLeftQuarterTurn160DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_SMALL_CURVE + 0)),
                    { 0, 0, height }, { { 2, 2, height }, { 28, 28, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_SMALL_CURVE + 1)),
                    { 0, 0, height }, { { 2, 2, height + 99 }, { 28, 28, 1 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_SMALL_CURVE + 2)),
                    { 0, 0, height }, { { 2, 2, height }, { 28, 28, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_SMALL_CURVE + 3)),
                    { 0, 0, height }, { { 2, 2, height + 99 }, { 28, 28, 1 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_SMALL_CURVE + 4)),
                    { 0, 0, height }, { { 2, 2, height }, { 28, 28, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_SMALL_CURVE + 5)),
                    { 0, 0, height }, { { 2, 2, height + 99 }, { 28, 28, 1 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_SMALL_CURVE + 6)),
                    { 0, 0, height }, { { 2, 2, height }, { 28, 28, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_SMALL_CURVE + 7)),
                    { 0, 0, height }, { { 2, 2, height + 99 }, { 28, 28, 1 } });
                break;
        }
        TrackPaintUtilLeftQuarterTurn1TileTunnel(
            session, TunnelGroup::Standard, direction, height, -8, TunnelSubType::SlopeStart, +56, TunnelSubType::SlopeEnd);
        PaintUtilSetSegmentSupportHeight(session, kSegmentsAll, 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 104);
    }

    static void TrackRightQuarterTurn160DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_SMALL_CURVE + 8)),
                    { 0, 0, height }, { { 2, 2, height }, { 28, 28, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_SMALL_CURVE + 9)),
                    { 0, 0, height }, { { 2, 2, height + 99 }, { 28, 28, 1 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_SMALL_CURVE + 10)),
                    { 0, 0, height }, { { 2, 2, height }, { 28, 28, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_SMALL_CURVE + 11)),
                    { 0, 0, height }, { { 2, 2, height + 99 }, { 28, 28, 1 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_SMALL_CURVE + 12)),
                    { 0, 0, height }, { { 2, 2, height }, { 28, 28, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_SMALL_CURVE + 13)),
                    { 0, 0, height }, { { 2, 2, height + 99 }, { 28, 28, 1 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_SMALL_CURVE + 14)),
                    { 0, 0, height }, { { 2, 2, height }, { 28, 28, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_STEEP_SMALL_CURVE + 15)),
                    { 0, 0, height }, { { 2, 2, height + 99 }, { 28, 28, 1 } });
                break;
        }
        TrackPaintUtilRightQuarterTurn1TileTunnel(
            session, TunnelGroup::Standard, direction, height, -8, TunnelSubType::SlopeStart, +56, TunnelSubType::SlopeEnd);
        PaintUtilSetSegmentSupportHeight(session, kSegmentsAll, 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 104);
    }

    static void TrackLeftQuarterTurn160DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackRightQuarterTurn160DegUp(session, ride, trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void TrackRightQuarterTurn160DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackLeftQuarterTurn160DegUp(session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void TrackLeftQuarterTurn190DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL_TWIST + 0)),
                            { 0, 0, height }, { { 4, 6, height + 8 }, { 2, 20, 63 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL_TWIST + 1)),
                            { 0, 0, height }, { { 4, 6, height + 8 }, { 2, 20, 63 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL_TWIST + 2)),
                            { 0, 0, height }, { { 24, 6, height + 8 }, { 2, 20, 63 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL_TWIST + 3)),
                            { 0, 0, height }, { { 24, 6, height + 8 }, { 2, 20, 63 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL_TWIST + 4)),
                            { 0, 0, height }, { { 4, 6, height + 8 }, { 2, 20, 63 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL_TWIST + 5)),
                            { 0, 0, height }, { { 24, 6, height + 8 }, { 2, 20, 63 } });
                        break;
                }
                PaintUtilSetVerticalTunnel(session, height + 96);
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 96);
                break;
            case 1:
                break;
        }
    }

    static void TrackRightQuarterTurn190DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL_TWIST + 6)),
                            { 0, 0, height }, { { 4, 6, height + 8 }, { 2, 20, 63 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL_TWIST + 7)),
                            { 0, 0, height }, { { 24, 6, height + 8 }, { 2, 20, 63 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL_TWIST + 8)),
                            { 0, 0, height }, { { 24, 6, height + 8 }, { 2, 20, 63 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL_TWIST + 9)),
                            { 0, 0, height }, { { 4, 6, height + 8 }, { 2, 20, 63 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL_TWIST + 10)),
                            { 0, 0, height }, { { 24, 6, height + 8 }, { 2, 20, 63 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_VERTICAL_TWIST + 11)),
                            { 0, 0, height }, { { 4, 6, height + 8 }, { 2, 20, 63 } });
                        break;
                }
                PaintUtilSetVerticalTunnel(session, height + 96);
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 96);
                break;
            case 1:
                break;
        }
    }

    static void TrackLeftQuarterTurn190DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackRightQuarterTurn190DegUp(session, ride, trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void TrackRightQuarterTurn190DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackLeftQuarterTurn190DegUp(session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void Track25DegUpToLeftBanked25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 0)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 1)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 2)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 3)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 4)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void Track25DegUpToRightBanked25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 5)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 6)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 7)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 8)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 9)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void TrackLeftBanked25DegUpTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 10)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 11)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 12)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 13)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 14)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void TrackRightBanked25DegUpTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 15)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 16)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 17)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 18)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 19)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void TrackLeftBanked25DegDownTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        Track25DegUpToRightBanked25DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackRightBanked25DegDownTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        Track25DegUpToLeftBanked25DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void Track25DegDownToLeftBanked25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackRightBanked25DegUpTo25DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void Track25DegDownToRightBanked25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackLeftBanked25DegUpTo25DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackLeftBankedFlatToLeftBanked25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 20)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 21)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 22)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 23)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 48);
    }

    static void TrackRightBankedFlatToRightBanked25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 24)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 25)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 26)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 27)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 48);
    }

    static void TrackLeftBanked25DegUpToLeftBankedFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 28)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 29)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 30)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 31)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::FlatTo25Deg);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 40);
    }

    static void TrackRightBanked25DegUpToRightBankedFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 32)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 33)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 34)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 35)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::FlatTo25Deg);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 40);
    }

    static void TrackLeftBankedFlatToLeftBanked25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackRightBanked25DegUpToRightBankedFlat(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackRightBankedFlatToRightBanked25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackLeftBanked25DegUpToLeftBankedFlat(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackLeftBanked25DegDownToLeftBankedFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackRightBankedFlatToRightBanked25DegUp(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackRightBanked25DegDownToRightBankedFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackLeftBankedFlatToLeftBanked25DegUp(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void Track25DegUpLeftBanked(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 36)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 37)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 38)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 39)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void Track25DegUpRightBanked(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 40)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 41)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 42)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 43)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void Track25DegDownLeftBanked(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        Track25DegUpRightBanked(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void Track25DegDownRightBanked(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        Track25DegUpLeftBanked(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackFlatToLeftBanked25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 44)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 45)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 46)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 47)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 48)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 48);
    }

    static void TrackFlatToRightBanked25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 49)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 50)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 51)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 52)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 53)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 48);
    }

    static void TrackLeftBanked25DegUpToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 54)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 55)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 56)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 57)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 58)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::FlatTo25Deg);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 40);
    }

    static void TrackRightBanked25DegUpToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 59)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 60)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 61)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 62)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SLOPE_BANK_TRANSITION + 63)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::FlatTo25Deg);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 40);
    }

    static void TrackFlatToLeftBanked25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackRightBanked25DegUpToFlat(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackFlatToRightBanked25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackLeftBanked25DegUpToFlat(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackLeftBanked25DegDownToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackFlatToRightBanked25DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackRightBanked25DegDownToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackFlatToLeftBanked25DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackLeftBankedQuarterTurn3Tile25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE_BANKED + 0)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE_BANKED + 2)),
                            { 0, 6, height }, { { 0, 27, height }, { 32, 1, 34 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE_BANKED + 4)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE_BANKED + 6)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE_BANKED + 1)),
                            { 6, 0, height }, { 20, 32, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE_BANKED + 3)),
                            { 6, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE_BANKED + 5)),
                            { 6, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE_BANKED + 7)),
                            { 6, 0, height }, { 20, 32, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackRightBankedQuarterTurn3Tile25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE_BANKED + 8)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE_BANKED + 10)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE_BANKED + 12)),
                            { 0, 6, height }, { { 0, 27, height }, { 32, 1, 34 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE_BANKED + 14)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE_BANKED + 9)),
                            { 6, 0, height }, { 20, 32, 3 });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE_BANKED + 11)),
                            { 6, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE_BANKED + 13)),
                            { 6, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 10, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_SMALL_CURVE_BANKED + 15)),
                            { 6, 0, height }, { 20, 32, 3 });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                        break;
                }
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackLeftBankedQuarterTurn3Tile25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
        TrackRightBankedQuarterTurn3Tile25DegUp(
            session, ride, trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void TrackRightBankedQuarterTurn3Tile25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
        TrackLeftBankedQuarterTurn3Tile25DegUp(
            session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void TrackLeftBankedQuarterTurn525DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 5)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 10)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 15)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 1)),
                            { 0, 0, height }, { 32, 16, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 6)),
                            { 0, 0, height }, { { 30, 30, height }, { 1, 1, 34 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 11)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 16)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 2)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 7)),
                            { 0, 0, height }, { { 30, 30, height }, { 1, 1, 34 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 12)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 17)),
                            { 0, 0, height }, { 16, 16, 3 });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 4:
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 3)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 8)),
                            { 0, 0, height }, { { 30, 30, height }, { 1, 1, 34 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 13)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 18)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 4)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 9)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 14)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 19)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackRightBankedQuarterTurn525DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 20)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 25)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 30)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 35)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 21)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 26)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 31)),
                            { 0, 0, height }, { { 30, 30, height }, { 1, 1, 34 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 36)),
                            { 0, 0, height }, { 32, 16, 3 });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 22)),
                            { 0, 0, height }, { 16, 16, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 27)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 32)),
                            { 0, 0, height }, { { 30, 30, height }, { 1, 1, 34 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 37)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 4:
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 23)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 28)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 33)),
                            { 0, 0, height }, { { 30, 30, height }, { 1, 1, 34 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 38)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 24)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 29)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 34)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 39)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackLeftBankedQuarterTurn525DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
        TrackRightBankedQuarterTurn525DegUp(
            session, ride, trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void TrackRightBankedQuarterTurn525DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
        TrackLeftBankedQuarterTurn525DegUp(
            session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void TrackSBendLeft(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 4)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 3)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 7)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 1)),
                            { 0, 0, height }, { 32, 26, 3 });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopLeftSide, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 5)),
                            { 0, 0, height }, { 32, 26, 3 });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopRightSide, 1, height - 1, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 2)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 26, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 6)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 26, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 2)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 26, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 6)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 26, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 1)),
                            { 0, 0, height }, { 32, 26, 3 });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopLeftSide, 0, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 5)),
                            { 0, 0, height }, { 32, 26, 3 });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopRightSide, 1, height - 1, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 3)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 7)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 4)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                switch (direction)
                {
                    case 1:
                        PaintUtilPushTunnelRight(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 2:
                        PaintUtilPushTunnelLeft(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackSBendRight(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 8)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 12)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 11)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 15)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 9)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 26, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomRightSide, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 13)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 26, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomLeftSide, 0, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 10)),
                            { 0, 0, height }, { 32, 26, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 14)),
                            { 0, 0, height }, { 32, 26, 3 });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 10)),
                            { 0, 0, height }, { 32, 26, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 14)),
                            { 0, 0, height }, { 32, 26, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 9)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 26, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomRightSide, 0, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 13)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 26, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomLeftSide, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 11)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 15)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 8)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_S_BEND + 12)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                switch (direction)
                {
                    case 1:
                        PaintUtilPushTunnelRight(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 2:
                        PaintUtilPushTunnelLeft(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackLeftHalfBankedHelixUpSmall(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 1)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 4)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 7)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 11)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 2, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 2)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 5)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 8)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 12)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 3)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 6)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 9)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 10)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 13)),
                            { 0, 0, height }, { { 6, 0, height + 8 }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 11)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 0)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 1)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 4)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 7)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 2, height, session.SupportColours);
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 5:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 12)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 2)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 5)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 8)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 7:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 13)),
                            { 0, 0, height }, { { 0, 6, height + 8 }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 3)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 6)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 9)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 10)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackRightHalfBankedHelixUpSmall(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 14)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 17)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 21)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 24)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 25)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 2, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 15)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 18)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 22)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 26)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 16)),
                            { 0, 0, height }, { { 6, 0, height + 8 }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 19)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 20)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 23)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 27)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 17)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 21)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 24)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 25)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 14)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 2, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 5:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 18)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 22)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 26)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 15)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 7:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 19)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 20)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 23)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 27)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_HELIX + 16)),
                            { 0, 0, height }, { { 0, 6, height + 8 }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackLeftHalfBankedHelixDownSmall(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackSequence >= 4)
        {
            trackSequence -= 4;
            direction = (direction - 1) & 3;
        }
        trackSequence = kMapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
        TrackRightHalfBankedHelixUpSmall(session, ride, trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void TrackRightHalfBankedHelixDownSmall(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackSequence >= 4)
        {
            trackSequence -= 4;
            direction = (direction + 1) & 3;
        }
        trackSequence = kMapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
        TrackLeftHalfBankedHelixUpSmall(session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void TrackLeftHalfBankedHelixUpLarge(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 1)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 6)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 11)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 17)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 1, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 2)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 7)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 32, 16, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 12)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 18)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 3)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 8)),
                            { 0, 0, height }, { { 16, 16, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 13)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 19)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 4)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 9)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 16, 32, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 14)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 20)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 5)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 10)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 15)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 16)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 21)),
                            { 0, 0, height }, { { 6, 0, height + 8 }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 7, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 7:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 17)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 0)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 1)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 6)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 11)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 1, height, session.SupportColours);
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 8:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 9:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 18)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 2)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 7)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 16, 32, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 12)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 10:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 19)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 3)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 8)),
                            { 0, 0, height }, { { 16, 16, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 13)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 11:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 12:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 20)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 4)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 9)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 32, 16, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 14)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 13:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 21)),
                            { 0, 0, height }, { { 0, 6, height + 8 }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 5)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 10)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 15)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 16)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 7, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }
    static void TrackRightHalfBankedHelixUpLarge(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 22)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 27)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 33)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 38)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 39)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 1, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 23)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 28)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 34)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 32, 16, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 40)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 24)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 29)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 35)),
                            { 0, 0, height }, { { 16, 16, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 41)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 25)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 30)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 36)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 16, 32, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 42)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 26)),
                            { 0, 0, height }, { { 6, 0, height + 8 }, { 20, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 7, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 31)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 32)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 37)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 7, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 43)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 7, height, session.SupportColours);
                        break;
                }
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 7:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 27)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 33)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 38)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 39)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 22)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 1, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 8:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 9:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 28)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 34)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 16, 32, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 40)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 23)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 10:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 29)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 35)),
                            { 0, 0, height }, { { 16, 16, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 41)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 24)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 11:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 12:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 30)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 36)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 32, 16, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 42)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 25)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 13:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 31)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 32)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 37)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 7, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 43)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 7, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HELIX + 26)),
                            { 0, 0, height }, { { 0, 6, height + 8 }, { 32, 20, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 7, height, session.SupportColours);
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void TrackLeftHalfBankedHelixDownLarge(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackSequence >= 7)
        {
            trackSequence -= 7;
            direction = (direction - 1) & 3;
        }
        trackSequence = kMapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
        TrackRightHalfBankedHelixUpLarge(session, ride, trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void TrackRightHalfBankedHelixDownLarge(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackSequence >= 7)
        {
            trackSequence -= 7;
            direction = (direction + 1) & 3;
        }
        trackSequence = kMapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
        TrackLeftHalfBankedHelixUpLarge(session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void TrackLeftBarrelRollUpToDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 1)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 4, height + 1, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 6)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 7)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height + 1, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 12)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 13)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height + 1, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 18)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 19)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 2)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 3)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 0 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 8)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 9)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 0 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 14)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 15)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 0 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 20)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 21)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 4)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 5)),
                            { 0, 0, height }, { { 0, 6, height + 44 }, { 32, 20, 0 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 10)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 11)),
                            { 0, 0, height }, { { 0, 6, height + 44 }, { 32, 20, 0 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 16)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 17)),
                            { 0, 0, height }, { { 0, 6, height + 44 }, { 32, 20, 0 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 22)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 23)),
                            { 0, 0, height }, { { 0, 6, height + 44 }, { 32, 20, 0 } });
                        break;
                }
                switch (direction)
                {
                    case 1:
                        PaintUtilPushTunnelRight(session, height, TunnelGroup::Inverted, TunnelSubType::Flat);
                        break;
                    case 2:
                        PaintUtilPushTunnelLeft(session, height, TunnelGroup::Inverted, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackRightBarrelRollUpToDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 24)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 25)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 30)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 31)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height + 1, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 36)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 37)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height + 1, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 42)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 43)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height + 1, session.SupportColours);
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 26)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 27)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 0 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 32)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 33)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 0 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 38)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 39)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 0 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 44)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 45)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 28)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 29)),
                            { 0, 0, height }, { { 0, 6, height + 44 }, { 32, 20, 0 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 34)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 35)),
                            { 0, 0, height }, { { 0, 6, height + 44 }, { 32, 20, 0 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 40)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 41)),
                            { 0, 0, height }, { { 0, 6, height + 44 }, { 32, 20, 0 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 46)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BARREL_ROLL + 47)),
                            { 0, 0, height }, { { 0, 6, height + 44 }, { 32, 20, 0 } });
                        break;
                }
                switch (direction)
                {
                    case 1:
                        PaintUtilPushTunnelRight(session, height, TunnelGroup::Inverted, TunnelSubType::Flat);
                        break;
                    case 2:
                        PaintUtilPushTunnelLeft(session, height, TunnelGroup::Inverted, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackLeftBarrelRollDownToUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackLeftBarrelRollUpToDown(session, ride, 2 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackRightBarrelRollDownToUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackRightBarrelRollUpToDown(session, ride, 2 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackHalfLoopUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_HALF_LOOP + 0)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_HALF_LOOP + 4)),
                            { 0, 6, height }, { 32, 20, 11 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_HALF_LOOP + 8)),
                            { 0, 6, height }, { 32, 20, 9 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_HALF_LOOP + 12)),
                            { 0, 6, height }, { 32, 20, 7 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_HALF_LOOP + 1)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 20, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_HALF_LOOP + 5)),
                            { 0, 14, height }, { { 28, 6, height }, { 3, 20, 63 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 15, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_HALF_LOOP + 9)),
                            { 0, 6, height }, { { 28, 6, height }, { 3, 20, 63 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 16, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_HALF_LOOP + 13)),
                            { 0, 6, height }, { 32, 20, 3 });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 16, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(session, kSegmentsAll, 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_HALF_LOOP + 2)),
                            { 16, 0, height }, { 2, 16, 119 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_HALF_LOOP + 6)),
                            { 12, 0, height }, { { 12, 0, height }, { 3, 16, 119 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_HALF_LOOP + 10)),
                            { 10, 16, height }, { 4, 12, 119 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_HALF_LOOP + 14)),
                            { 16, 16, height }, { { 15, 6, height }, { 2, 16, 119 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 168);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_HALF_LOOP + 3)),
                            { 0, 0, height + 32 }, { 32, 16, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_HALF_LOOP + 7)),
                            { 0, 0, height + 32 }, { 32, 16, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_HALF_LOOP + 11)),
                            { 0, 16, height + 32 }, { 32, 12, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_HALF_LOOP + 15)),
                            { 0, 16, height + 32 }, { 32, 12, 3 });
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackHalfLoopDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackHalfLoopUp(session, ride, 3 - trackSequence, direction, height, trackElement, supportType);
    }

    static void TrackBrakes(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BRAKE + 0)), { 0, 0, height },
                    { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BRAKE + 1)), { 0, 0, height },
                    { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
        }
        PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
    }

    static void TrackOnRidePhoto(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackPaintUtilOnridePhotoPlatformPaint(session, direction, height, supportType.metal);

        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT + 0)), { 0, 0, height },
                    { { 0, 6, height + 3 }, { 32, 20, 0 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT + 1)), { 0, 0, height },
                    { { 0, 6, height + 3 }, { 32, 20, 0 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT + 0)), { 0, 0, height },
                    { { 0, 6, height + 3 }, { 32, 20, 0 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT + 1)), { 0, 0, height },
                    { { 0, 6, height + 3 }, { 32, 20, 0 } });
                break;
        }
        TrackPaintUtilOnridePhotoPaint(session, direction, height + 3, trackElement);
        PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Square, TunnelSubType::Flat);
        PaintUtilSetSegmentSupportHeight(session, kSegmentsAll, 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 48);
    }

    static void TrackFlatTo60DegUpLongBase(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 4)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 8)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 12)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
                {
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                }
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 1)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 5)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 9)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 13)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
                {
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 9, height, session.SupportColours);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 2)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 6)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 10)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 14)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
                {
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 10, height, session.SupportColours);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 3)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 7)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 98 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 11)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 98 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 15)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
                {
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 19, height, session.SupportColours);
                }
                switch (direction)
                {
                    case 1:
                        PaintUtilPushTunnelRight(session, height + 24, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                    case 2:
                        PaintUtilPushTunnelLeft(session, height + 24, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 80);
                break;
        }
    }
    static void Track60DegUpToFlatLongBase(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 16)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 20)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 98 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 24)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 98 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 28)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
                {
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 24, height, session.SupportColours);
                }
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 80);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 17)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 21)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 25)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 29)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
                {
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 18, height, session.SupportColours);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 80);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 18)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 22)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 26)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 30)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
                {
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 13, height, session.SupportColours);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 19)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 23)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 27)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_FLAT_TO_STEEP + 31)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
                {
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 5, height, session.SupportColours);
                }
                switch (direction)
                {
                    case 1:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 2:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 40);
                break;
        }
    }

    static void TrackFlatTo60DegDownLongBase(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        Track60DegUpToFlatLongBase(session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void Track60DegDownToFlatLongBase(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackFlatTo60DegUpLongBase(session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackBlockBrakes(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BLOCK_BRAKE + 0)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_BLOCK_BRAKE + 1)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
        }
        PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
    }

    static void TrackLeftCorkscrewUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 0)),
                            { 0, 0, height }, { { 0, 6, height + 4 }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 3)),
                            { 0, 0, height }, { { 0, 6, height + 4 }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 6)),
                            { 0, 0, height }, { { 0, 6, height + 4 }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 9)),
                            { 0, 0, height }, { { 0, 6, height + 4 }, { 32, 20, 3 } });
                        break;
                }

                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 2, height, session.SupportColours);

                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 1)),
                            { 0, 0, height }, { { 6, 6, height + 10 }, { 20, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 4)),
                            { 0, 0, height }, { { 6, 6, height + 10 }, { 20, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 7)),
                            { 0, 0, height }, { { 6, 6, height + 10 }, { 20, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 10)),
                            { 0, 0, height }, { { 6, 6, height + 10 }, { 20, 20, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(session, kSegmentsAll, 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 2)),
                            { 0, 0, height }, { { 6, 0, height + 24 }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 5)),
                            { 0, 0, height }, { { 6, 0, height + 24 }, { 20, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 8)),
                            { 0, 0, height }, { { 6, 0, height + 24 }, { 20, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 11)),
                            { 0, 0, height }, { { 6, 0, height + 24 }, { 20, 32, 3 } });
                        break;
                }

                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height + 33, session.SupportColours);

                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackRightCorkscrewUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 12)),
                            { 0, 0, height }, { { 0, 6, height + 4 }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 15)),
                            { 0, 0, height }, { { 0, 6, height + 4 }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 18)),
                            { 0, 0, height }, { { 0, 6, height + 4 }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 21)),
                            { 0, 0, height }, { { 0, 6, height + 4 }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 2, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 13)),
                            { 0, 0, height }, { { 6, 6, height + 10 }, { 20, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 16)),
                            { 0, 0, height }, { { 6, 6, height + 10 }, { 20, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 19)),
                            { 0, 0, height }, { { 6, 6, height + 10 }, { 20, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 22)),
                            { 0, 0, height }, { { 6, 6, height + 10 }, { 20, 20, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(session, kSegmentsAll, 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 14)),
                            { 0, 0, height }, { { 6, 0, height + 24 }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 17)),
                            { 0, 0, height }, { { 6, 0, height + 24 }, { 20, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 20)),
                            { 0, 0, height }, { { 6, 0, height + 24 }, { 20, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_CORKSCREW + 23)),
                            { 0, 0, height }, { { 6, 0, height + 24 }, { 20, 32, 3 } });
                        break;
                }

                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height + 33, session.SupportColours);

                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackLeftCorkscrewDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackRightCorkscrewUp(session, ride, 2 - trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void TrackRightCorkscrewDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackLeftCorkscrewUp(session, ride, 2 - trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void TrackLeftLargeCorkscrewUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 0)),
                            { 0, 0, height }, { { 0, 6, height + 4 }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 5)),
                            { 0, 0, height }, { { 0, 6, height + 4 }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 10)),
                            { 0, 0, height }, { { 0, 6, height + 4 }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 15)),
                            { 0, 0, height }, { { 0, 6, height + 4 }, { 32, 20, 3 } });
                        break;
                }

                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 2, height, session.SupportColours);

                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 40);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 1)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopRightSide, 34, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 6)),
                            { 0, 0, height }, { { 0, 29, height }, { 26, 1, 32 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomRightSide, 22, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 11)),
                            { 0, 0, height }, { { 0, 6, height }, { 20, 20, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomLeftSide, 24, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 16)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopLeftSide, 16, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 2)),
                            { 0, 0, height }, { { 0, 31, height }, { 48, 1, 64 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 7)),
                            { 0, 0, height }, { { 2, 2, height + 40 }, { 28, 28, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 12)),
                            { 0, 0, height }, { { 2, 2, height + 48 }, { 28, 28, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 17)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 4:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomLeftSide, PaintSegment::bottomCorner,
                            PaintSegment::topLeftSide, PaintSegment::centre, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 3)),
                            { 0, 0, height }, { { 2, 2, height + 50 }, { 28, 28, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopLeftSide, 0, height + 56, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 8)),
                            { 0, 0, height }, { { 2, 2, height + 50 }, { 28, 28, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopRightSide, 0, height + 56,
                            session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 13)),
                            { 0, 0, height }, { { 2, 2, height + 50 }, { 24, 28, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomRightSide, 0, height + 56,
                            session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 18)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomLeftSide, 0, height + 56,
                            session.SupportColours);
                        break;
                }

                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 4)),
                            { 0, 0, height }, { { 2, 2, height + 40 }, { 28, 28, 1 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 9)),
                            { 0, 0, height }, { { 2, 2, height + 40 }, { 28, 28, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 14)),
                            { 0, 0, height }, { { 2, 2, height + 40 }, { 26, 28, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 19)),
                            { 0, 0, height }, { { 2, 2, height + 40 }, { 26, 28, 1 } });
                        break;
                }

                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomLeftSide, PaintSegment::bottomCorner,
                            PaintSegment::topLeftSide, PaintSegment::centre, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackRightLargeCorkscrewUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 20)),
                            { 0, 0, height }, { { 0, 6, height + 4 }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 25)),
                            { 0, 0, height }, { { 0, 6, height + 4 }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 30)),
                            { 0, 0, height }, { { 0, 6, height + 4 }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 35)),
                            { 0, 0, height }, { { 0, 6, height + 4 }, { 32, 20, 3 } });
                        break;
                }

                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 2, height, session.SupportColours);

                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomRightSide, PaintSegment::centre, PaintSegment::rightCorner,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 40);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 21)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopRightSide, 16, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 26)),
                            { 0, 0, height }, { { 0, 6, height }, { 20, 20, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomRightSide, 24, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 31)),
                            { 0, 0, height }, { { 0, 29, height }, { 26, 1, 32 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomLeftSide, 22, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 36)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopLeftSide, 34, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::bottomRightSide, PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 22)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 27)),
                            { 0, 0, height }, { { 2, 2, height + 48 }, { 28, 28, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 32)),
                            { 0, 0, height }, { { 2, 2, height + 48 }, { 28, 28, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 37)),
                            { 0, 0, height }, { { 0, 31, height }, { 48, 1, 64 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 4:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomLeftSide, PaintSegment::bottomCorner,
                            PaintSegment::topLeftSide, PaintSegment::centre, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 23)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomRightSide, 0, height + 56,
                            session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 28)),
                            { 0, 0, height }, { { 2, 2, height + 50 }, { 24, 28, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomLeftSide, 0, height + 56,
                            session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 33)),
                            { 0, 0, height }, { { 2, 2, height + 50 }, { 28, 28, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopLeftSide, 0, height + 56, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 38)),
                            { 0, 0, height }, { { 2, 2, height + 50 }, { 28, 28, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopRightSide, 0, height + 56,
                            session.SupportColours);
                        break;
                }
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 24)),
                            { 0, 0, height }, { { 2, 2, height + 40 }, { 26, 28, 1 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 29)),
                            { 0, 0, height }, { { 2, 2, height + 40 }, { 26, 28, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 34)),
                            { 0, 0, height }, { { 2, 2, height + 40 }, { 28, 28, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_CORKSCREW + 39)),
                            { 0, 0, height }, { { 2, 2, height + 40 }, { 28, 28, 1 } });
                        break;
                }

                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomLeftSide, PaintSegment::bottomCorner,
                            PaintSegment::topLeftSide, PaintSegment::centre, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackLeftLargeCorkscrewDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackRightLargeCorkscrewUp(session, ride, 5 - trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void TrackRightLargeCorkscrewDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackLeftLargeCorkscrewUp(session, ride, 5 - trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void TrackLeftMediumHalfLoopUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 5)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 10)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 15)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }

                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 7, height, session.SupportColours);

                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 1)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 6)),
                            { 0, 0, height }, { { 0, 31, height }, { 0, 32, 64 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 11)), { 0, 0, height },
                            { { 0, 32, height }, { 40, 0, 64 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 16)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }

                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::topLeftSide, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::topCorner),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 2)),
                            { 0, 0, height }, { { 0, 0, height + 2 }, { 32, 32, 0 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopLeftSide, 14, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 7)),
                            { 0, 0, height }, { { 29, 0, height }, { 1, 32, 96 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopRightSide, 16, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 12)), { 0, 0, height },
                            { { 31, 0, height }, { 1, 32, 96 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomRightSide, 18, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 17)), { 0, 0, height },
                            { { 0, 0, height }, { 32, 32, 0 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomLeftSide, 14, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::topLeftSide, PaintSegment::centre,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 144);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 3)),
                            { 0, 0, height }, { { 2, 0, height }, { 1, 32, 160 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 8)),
                            { 0, 0, height }, { { 30, 0, height }, { 0, 32, 160 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 13)), { 0, 0, height },
                            { { 29, 0, height }, { 1, 32, 160 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 18)), { 0, 0, height },
                            { { 0, 0, height }, { 1, 32, 160 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::bottomRightSide, PaintSegment::centre,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 144);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 4)),
                            { 0, 0, height }, { { 0, 2, height + 48 }, { 32, 32, 1 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 9)),
                            { 0, 0, height }, { { 0, 2, height + 48 }, { 32, 32, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 14)), { 0, 0, height },
                            { { 0, 0, height + 48 }, { 32, 32, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 19)), { 0, 0, height },
                            { { 0, 0, height + 48 }, { 32, 32, 1 } });
                        break;
                }

                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topRightSide, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomCorner, PaintSegment::bottomRightSide, PaintSegment::rightCorner),
                        direction),
                    0xFFFF, 0);

                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height + 16, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackRightMediumHalfLoopUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 20)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 25)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 30)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 35)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }

                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 7, height, session.SupportColours);

                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 21)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 26)), { 0, 0, height },
                            { { 0, 32, height }, { 40, 0, 64 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 31)), { 0, 0, height },
                            { { 0, 31, height }, { 0, 32, 64 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 36)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }

                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topRightSide, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomCorner, PaintSegment::bottomRightSide, PaintSegment::rightCorner),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 22)), { 0, 0, height },
                            { { 0, 0, height }, { 32, 32, 0 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomRightSide, 14, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 27)), { 0, 0, height },
                            { { 30, 0, height }, { 0, 32, 96 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomLeftSide, 18, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 32)), { 0, 0, height },
                            { { 29, 0, height }, { 0, 32, 96 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopLeftSide, 16, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 37)), { 0, 0, height },
                            { { 0, 0, height + 2 }, { 32, 32, 0 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopRightSide, 14, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::bottomRightSide, PaintSegment::centre,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 144);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 23)), { 0, 0, height },
                            { { 0, 0, height }, { 1, 32, 160 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 28)), { 0, 0, height },
                            { { 29, 0, height }, { 0, 32, 160 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 33)), { 0, 0, height },
                            { { 30, 0, height }, { 0, 32, 160 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 38)), { 0, 0, height },
                            { { 2, 0, height }, { 1, 32, 160 } });
                        break;
                }

                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 144);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 24)), { 0, 0, height },
                            { { 0, 0, height + 48 }, { 32, 32, 1 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 29)), { 0, 0, height },
                            { { 0, 0, height + 48 }, { 32, 32, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 34)), { 0, 0, height },
                            { { 0, 0, height + 48 }, { 32, 32, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_MEDIUM_HALF_LOOP + 39)), { 0, 0, height },
                            { { 0, 0, height + 48 }, { 32, 32, 1 } });
                        break;
                }

                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::topLeftSide, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::topCorner),
                        direction),
                    0xFFFF, 0);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height + 16, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackLeftMediumHalfLoopDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackRightMediumHalfLoopUp(session, ride, 4 - trackSequence, direction, height, trackElement, supportType);
    }

    static void TrackRightMediumHalfLoopDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackLeftMediumHalfLoopUp(session, ride, 4 - trackSequence, direction, height, trackElement, supportType);
    }

    static void TrackLeftZeroGRollUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 1)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 1 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 4)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 5)),
                            { 0, 0, height }, { { 0, 31, height }, { 32, 1, 32 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 8)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 12)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 14, height, session.SupportColours);
                        break;
                }

                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 40);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 2)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 1 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 6)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 9)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 13)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::topLeftSide, PaintSegment::topCorner,
                            PaintSegment::bottomLeftSide, PaintSegment::centre, PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 3)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 1 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 7)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 10)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 11)),
                            { 0, 0, height }, { { 0, 6, height + 33 }, { 32, 20, 0 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 14)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 15)),
                            { 0, 0, height }, { { 0, 6, height + 33 }, { 32, 20, 0 } });
                        break;
                }
                switch (direction)
                {
                    case 1:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 2:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::topLeftSide, PaintSegment::topCorner,
                            PaintSegment::bottomLeftSide, PaintSegment::centre, PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height + 38, session.SupportColours);
                PaintUtilSetGeneralSupportHeight(session, height + 40);
                break;
        }
    }

    static void TrackRightZeroGRollUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 16)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 14, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 20)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 24)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 25)),
                            { 0, 0, height }, { { 0, 31, height }, { 32, 1, 32 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 28)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 29)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 1 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 40);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 17)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 21)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 26)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 30)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 1 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::bottomRightSide, PaintSegment::rightCorner,
                            PaintSegment::bottomLeftSide, PaintSegment::centre, PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 18)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 19)),
                            { 0, 0, height }, { { 0, 6, height + 33 }, { 32, 20, 0 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 22)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 23)),
                            { 0, 0, height }, { { 0, 6, height + 33 }, { 32, 20, 0 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 27)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_ZERO_G_ROLL + 31)),
                            { 0, 0, height }, { { 0, 6, height + 28 }, { 32, 20, 1 } });
                        break;
                }
                switch (direction)
                {
                    case 1:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 2:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::bottomRightSide, PaintSegment::rightCorner,
                            PaintSegment::bottomLeftSide, PaintSegment::centre, PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height + 38, session.SupportColours);
                PaintUtilSetGeneralSupportHeight(session, height + 40);
                break;
        }
    }

    static void TrackLeftZeroGRollDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackLeftZeroGRollUp(session, ride, 2 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackRightZeroGRollDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackRightZeroGRollUp(session, ride, 2 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackLeftLargeZeroGRollUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 0)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 5)), { 0, 0, height },
                            { { 0, 30, height }, { 32, 0, 96 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 9)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 10)), { 0, 0, height },
                            { { 0, 30, height }, { 32, 0, 96 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 15)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 25, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 88);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 1)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 2)), { 0, 0, height },
                            { { 0, 30, height }, { 32, 0, 96 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 6)), { 0, 0, height },
                            { { 0, 30, height }, { 32, 0, 64 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 11)), { 0, 0, height },
                            { { 0, 2, height }, { 32, 0, 64 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 16)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 3)), { 0, 0, height },
                            { { 0, 30, height }, { 32, 0, 64 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 7)), { 0, 0, height },
                            { { 0, 30, height }, { 32, 0, 48 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 12)), { 0, 0, height },
                            { { 0, 2, height }, { 32, 0, 48 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 17)), { 0, 0, height },
                            { { 0, 2, height }, { 32, 0, 32 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 18)), { 0, 0, height },
                            { { 0, 6, height + 40 }, { 32, 20, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::topLeftSide, PaintSegment::topCorner,
                            PaintSegment::bottomLeftSide, PaintSegment::centre, PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 3:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::topLeftSide, PaintSegment::topCorner,
                            PaintSegment::bottomLeftSide, PaintSegment::centre, PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 4)), { 0, 0, height },
                            { { 0, 26, height }, { 26, 0, 20 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomLeftSide, 0, height + 28,
                            session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 8)), { 0, 0, height },
                            { { 0, 26, height }, { 26, 0, 20 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopLeftSide, 0, height + 28, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 13)), { 0, 0, height },
                            { { 0, 2, height }, { 32, 0, 32 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 14)), { 0, 0, height },
                            { { 0, 6, height + 40 }, { 32, 20, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopRightSide, 0, height + 28,
                            session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 19)), { 0, 0, height },
                            { { 0, 18, height }, { 32, 10, 20 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomRightSide, 0, height + 28,
                            session.SupportColours);
                        break;
                }
                switch (direction)
                {
                    case 1:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 2:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetGeneralSupportHeight(session, height + 40);
                break;
        }
    }

    static void TrackRightLargeZeroGRollUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 20)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 25)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 26)), { 0, 0, height },
                            { { 0, 30, height }, { 32, 0, 96 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 31)), { 0, 0, height },
                            { { 0, 30, height }, { 32, 0, 96 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 35)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 25, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 88);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 21)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 27)), { 0, 0, height },
                            { { 0, 2, height }, { 32, 0, 64 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 32)), { 0, 0, height },
                            { { 0, 30, height }, { 32, 0, 64 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 36)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 37)), { 0, 0, height },
                            { { 0, 30, height }, { 32, 0, 96 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 22)), { 0, 0, height },
                            { { 0, 2, height }, { 32, 0, 32 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 23)), { 0, 0, height },
                            { { 0, 6, height + 40 }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 28)), { 0, 0, height },
                            { { 0, 2, height }, { 32, 0, 48 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 33)), { 0, 0, height },
                            { { 0, 30, height }, { 32, 0, 48 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 38)), { 0, 0, height },
                            { { 0, 30, height }, { 32, 0, 64 } });
                        break;
                }

                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 3:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 24)), { 0, 0, height },
                            { { 0, 18, height }, { 32, 10, 20 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomLeftSide, 0, height + 28,
                            session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 29)), { 0, 0, height },
                            { { 0, 2, height }, { 32, 0, 32 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 30)), { 0, 0, height },
                            { { 0, 6, height + 40 }, { 32, 20, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopLeftSide, 0, height + 28, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 34)), { 0, 0, height },
                            { { 0, 26, height }, { 26, 0, 20 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopRightSide, 0, height + 28,
                            session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_ZERO_G_ROLL + 39)), { 0, 0, height },
                            { { 0, 26, height }, { 26, 0, 20 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomRightSide, 0, height + 28,
                            session.SupportColours);
                        break;
                }
                switch (direction)
                {
                    case 1:
                        PaintUtilPushTunnelRight(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 2:
                        PaintUtilPushTunnelLeft(session, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetGeneralSupportHeight(session, height + 40);
                break;
        }
    }

    static void TrackLeftLargeZeroGRollDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackLeftLargeZeroGRollUp(session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackRightLargeZeroGRollDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackRightLargeZeroGRollUp(session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void Track90DegToInvertedFlatQuarterLoopUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_QUARTER_LOOP + 0)),
                            { 0, 0, height }, { { 4, 6, height + 8 }, { 2, 20, 31 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_QUARTER_LOOP + 3)),
                            { 0, 0, height }, { { 24, 6, height + 8 }, { 2, 20, 31 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_QUARTER_LOOP + 6)),
                            { 0, 0, height }, { { 24, 6, height + 8 }, { 2, 20, 31 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_QUARTER_LOOP + 9)),
                            { 0, 0, height }, { { 4, 6, height + 8 }, { 2, 20, 31 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(PaintSegment::centre, PaintSegment::topLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 88);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_QUARTER_LOOP + 1)),
                            { 0, 0, height }, { { -8, 6, height }, { 2, 20, 31 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_QUARTER_LOOP + 4)),
                            { 0, 0, height }, { { 24, 6, height + 8 }, { 2, 20, 63 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_QUARTER_LOOP + 7)),
                            { 0, 0, height }, { { 24, 6, height + 8 }, { 2, 20, 63 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_QUARTER_LOOP + 10)),
                            { 0, 0, height }, { { -8, 6, height }, { 2, 20, 31 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(PaintSegment::centre, PaintSegment::topLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_QUARTER_LOOP + 2)),
                            { 0, 0, height }, { { 0, 6, height + 24 }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_QUARTER_LOOP + 5)),
                            { 0, 0, height }, { { 24, 6, height + 8 }, { 2, 20, 31 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_QUARTER_LOOP + 8)),
                            { 0, 0, height }, { { 24, 6, height + 8 }, { 2, 20, 31 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_QUARTER_LOOP + 11)),
                            { 0, 0, height }, { { 0, 6, height + 24 }, { 32, 20, 3 } });
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(PaintSegment::centre, PaintSegment::topLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackInvertedFlatTo90DegQuarterLoopDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        Track90DegToInvertedFlatQuarterLoopUp(session, ride, 2 - trackSequence, direction, height, trackElement, supportType);
    }

    static void TrackLeftBankToLeftQuarterTurn3Tile25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 0)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 2)),
                            { 0, 6, height }, { 32, 20, 3 });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 3)),
                            { 0, 6, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 5)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 7)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 1)),
                            { 6, 0, height }, { { 0, 6, height - 6 }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 4)),
                            { 6, 0, height }, { { 0, 6, height - 6 }, { 20, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 6)),
                            { 6, 0, height }, { { 0, 6, height - 6 }, { 20, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 8)),
                            { 6, 0, height }, { { 0, 6, height - 6 }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height - 6, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
        }
    }

    static void TrackRightBankToRightQuarterTurn3Tile25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 9)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 11)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 13)),
                            { 0, 6, height }, { 32, 20, 3 });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 14)),
                            { 0, 6, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 16)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 10)),
                            { 6, 0, height }, { { 0, 6, height - 6 }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 12)),
                            { 6, 0, height }, { { 0, 6, height - 6 }, { 20, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 15)),
                            { 6, 0, height }, { { 0, 6, height - 6 }, { 20, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 17)),
                            { 6, 0, height }, { { 0, 6, height - 6 }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height - 6, session.SupportColours);
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
        }
    }

    static void TrackLeftQuarterTurn3Tile25DegDownToLeftBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 12)),
                            { 0, 6, height }, { { 0, 6, height - 6 }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 15)),
                            { 0, 6, height }, { { 0, 6, height - 6 }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 17)),
                            { 0, 6, height }, { { 0, 6, height - 6 }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 10)),
                            { 0, 6, height }, { { 0, 6, height - 6 }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height - 6, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 11)),
                            { 6, 0, height }, { 20, 32, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 13)),
                            { 6, 0, height }, { 20, 32, 3 });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 14)),
                            { 6, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 16)),
                            { 6, 0, height }, { 20, 32, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 9)),
                            { 6, 0, height }, { 20, 32, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
        }
    }

    static void TrackRightQuarterTurn3Tile25DegDownToRightBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 8)),
                            { 0, 6, height }, { { 0, 6, height - 6 }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 1)),
                            { 0, 6, height }, { { 0, 6, height - 6 }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 4)),
                            { 0, 6, height }, { { 0, 6, height - 6 }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 6)),
                            { 0, 6, height }, { { 0, 6, height - 6 }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height - 6, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 7)),
                            { 6, 0, height }, { 20, 32, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 0)),
                            { 6, 0, height }, { 20, 32, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 2)),
                            { 6, 0, height }, { 20, 32, 3 });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 3)),
                            { 6, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_TURN_BANK_TRANSITION + 5)),
                            { 6, 0, height }, { 20, 32, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height, TunnelGroup::Standard, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
        }
    }

    static void TrackLeftLargeHalfLoopUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 7)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 14)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 21)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 1)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 15, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 8)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 9 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 9, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 15)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 9, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 22)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 2)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 9)),
                            { 0, 0, height }, { { 0, 0, height + 70 }, { 32, 16, 0 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 16)),
                            { 0, 0, height }, { { 0, 16, height + 70 }, { 32, 16, 0 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 23)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 88);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 3)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopLeftSide, 28, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 10)),
                            { 0, 0, height }, { { 0, 0, height + 200 }, { 32, 16, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopRightSide, 28, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 17)),
                            { 0, 0, height }, { { 0, 16, height + 200 }, { 32, 16, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomRightSide, 0, height + 28,
                            session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 24)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomLeftSide, 28, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 224);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 4)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 11)),
                            { 0, 0, height }, { { 0, 16, height + 110 }, { 16, 16, 0 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 18)),
                            { 0, 0, height }, { { 0, 0, height + 100 }, { 16, 16, 0 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 25)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 128);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 5)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 12)),
                            { 0, 0, height }, { { 0, 16, height + 200 }, { 32, 16, 0 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 19)),
                            { 0, 0, height }, { { 0, 0, height + 200 }, { 32, 16, 0 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 26)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 224);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 6)),
                            { 0, 0, height }, { { 0, 16, height + 32 }, { 32, 16, 0 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 13)),
                            { 0, 0, height }, { { 0, 16, height + 32 }, { 32, 16, 0 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 20)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 16, 0 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 27)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 16, 0 } });
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 40);
                break;
        }
    }

    static void TrackRightLargeHalfLoopUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 28)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 35)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 42)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 49)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 29)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 36)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 9, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 43)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 9 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 9, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 50)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 15, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 30)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 37)),
                            { 0, 0, height }, { { 0, 16, height + 70 }, { 32, 16, 0 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 44)),
                            { 0, 0, height }, { { 0, 0, height + 70 }, { 32, 16, 0 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 51)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 88);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 31)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomRightSide, 28, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 38)),
                            { 0, 0, height }, { { 0, 16, height + 200 }, { 32, 16, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomLeftSide, 0, height + 28,
                            session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 45)),
                            { 0, 0, height }, { { 0, 0, height + 200 }, { 32, 16, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopLeftSide, 28, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 52)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopRightSide, 28, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 224);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 32)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 39)),
                            { 0, 0, height }, { { 0, 0, height + 100 }, { 16, 16, 0 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 46)),
                            { 0, 0, height }, { { 0, 16, height + 110 }, { 16, 16, 0 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 53)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 128);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 33)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 40)),
                            { 0, 0, height }, { { 0, 0, height + 200 }, { 32, 16, 0 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 47)),
                            { 0, 0, height }, { { 0, 16, height + 200 }, { 32, 16, 0 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 54)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 224);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 34)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 16, 0 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 41)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 16, 0 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 48)),
                            { 0, 0, height }, { { 0, 16, height + 32 }, { 32, 16, 0 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_LARGE_HALF_LOOP + 55)),
                            { 0, 0, height }, { { 0, 16, height + 32 }, { 32, 16, 0 } });
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 40);
                break;
        }
    }

    static void TrackLeftLargeHalfLoopDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackRightLargeHalfLoopUp(session, ride, 6 - trackSequence, direction, height, trackElement, supportType);
    }

    static void TrackRightLargeHalfLoopDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackLeftLargeHalfLoopUp(session, ride, 6 - trackSequence, direction, height, trackElement, supportType);
    }

    static void TrackFlatTo60DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 0)),
                        { 0, 0, height }, { { 0, 2, height }, { 32, 27, 4 } });
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 2)),
                        { 0, 0, height }, { { 29, 4, height + 2 }, { 1, 24, 43 } });
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 1)),
                        { 0, 0, height }, { { 0, 4, height }, { 32, 2, 43 } });
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 0, height + 4, session.SupportColours);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 4)),
                        { 0, 0, height }, { { 29, 4, height + 2 }, { 1, 24, 43 } });
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 3)),
                        { 0, 0, height }, { { 0, 4, height }, { 32, 2, 43 } });
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 0, height + 4, session.SupportColours);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 5)),
                        { 0, 0, height }, { { 0, 2, height }, { 32, 27, 4 } });
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                    break;
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 0)),
                        { 0, 0, height }, { { 0, 2, height }, { 32, 27, 4 } });
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 2)),
                        { 0, 0, height }, { { 29, 4, height + 2 }, { 1, 24, 43 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 1)),
                        { 0, 0, height }, { { 0, 4, height }, { 32, 2, 43 } });
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 0, height + 4, session.SupportColours);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 4)),
                        { 0, 0, height }, { { 29, 4, height + 2 }, { 1, 24, 43 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 3)),
                        { 0, 0, height }, { { 0, 4, height }, { 32, 2, 43 } });
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 0, height + 4, session.SupportColours);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 5)),
                        { 0, 0, height }, { { 0, 2, height }, { 32, 27, 4 } });
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                    break;
            }
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height, TunnelGroup::Standard, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 24, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 64);
    }

    static void Track60DegUpToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 6)),
                        { 0, 0, height }, { { 0, 2, height }, { 32, 27, 4 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 7)),
                        { 0, 0, height }, { { 29, 4, height + 2 }, { 1, 24, 43 } });
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 8)),
                        { 0, 0, height }, { { 0, 4, height }, { 32, 2, 43 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 10)),
                        { 0, 0, height }, { { 29, 4, height + 2 }, { 1, 24, 43 } });
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 9)),
                        { 0, 0, height }, { { 0, 4, height }, { 32, 2, 43 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 11)),
                        { 0, 0, height }, { { 0, 2, height }, { 32, 27, 4 } });
                    break;
            }
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 16, height, session.SupportColours);
        }
        else
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 6)),
                        { 0, 0, height }, { { 0, 2, height }, { 32, 27, 4 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 7)),
                        { 0, 0, height }, { { 0, 4, height }, { 32, 2, 43 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 8)),
                        { 0, 0, height }, { { 29, 4, height + 2 }, { 1, 24, 43 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 10)),
                        { 0, 0, height }, { { 29, 4, height + 2 }, { 1, 24, 43 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 9)),
                        { 0, 0, height }, { { 0, 4, height }, { 32, 2, 43 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 11)),
                        { 0, 0, height }, { { 0, 2, height }, { 32, 27, 4 } });
                    break;
            }
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 16, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 24, TunnelGroup::Standard, TunnelSubType::Flat);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 72);
    }

    static void TrackFlatTo60DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        Track60DegUpToFlat(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void Track60DegDownToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackFlatTo60DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackDiagFlatTo60DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 15)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 15)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 12)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 12)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 14)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 14)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 7, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 13)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 7, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 7, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 7, height, session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 7, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 13)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 7, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 7, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 7, height, session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
        }
    }

    static void TrackDiag60DegUpToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 19)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 19)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 16)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 16)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 18)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 18)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 20, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 17)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 20, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 20, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 20, height,
                                session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 20, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 17)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 20, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 20, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 20, height,
                                session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackDiagFlatTo60DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 17)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 17)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 18)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 18)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 16)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 16)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 16, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 19)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 16, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 16, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 16, height,
                                session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 16, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 19)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 16, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 16, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 16, height,
                                session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackDiag60DegDownToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 13)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 13)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 14)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 14)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 12)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 12)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 5, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 15)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 5, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 5, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 5, height, session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 5, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_SMALL_FLAT_TO_STEEP + 15)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 5, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 5, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 5, height, session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
        }
    }

    // EnumsToFlags(PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::rightCorner, PaintSegment::bottomCorner,
    // PaintSegment::centre, PaintSegment::topLeftSide, PaintSegment::topRightSide, PaintSegment::bottomLeftSide,
    // PaintSegment::bottomRightSide);
    //     b4
    //   c8  cc
    // b8  c4  bc
    //   d0  d4
    //     c0

    //    0
    //  5   6
    // 1   4   2
    //  7   8
    //    3

    // 3102
    // 8756

    static void TrackLeftEighthToDiagUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 0)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 4)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 8)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 12)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 1)), { 0, 0, height },
                            { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 5)), { 0, 0, height },
                            { { 0, 0, height }, { 34, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 9)), { 0, 0, height },
                            { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 13)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 2)), { 0, 0, height },
                            { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 6)), { 0, 0, height },
                            { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 10)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 14)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 3)), { 0, 0, height },
                            { { 16, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 7)), { 0, 0, height },
                            { { 0, 16, height }, { 16, 18, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 11)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 15)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackRightEighthToDiagUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 16)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 20)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 24)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 28)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 17)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 21)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 25)),
                            { 0, 0, height }, { { 0, 0, height }, { 34, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 29)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 18)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 22)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 26)),
                            { 0, 0, height }, { { 4, 4, height }, { 28, 28, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 30)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 19)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 23)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 27)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 18, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 31)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackLeftEighthToOrthogonalUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 32)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 36)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 18, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 40)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 44)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 33)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 37)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 41)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 45)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 34)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 38)),
                            { 0, 0, height }, { { 0, 0, height }, { 34, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 42)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 46)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 35)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 39)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 43)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 47)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);

                if (direction == 1 || direction == 2)
                {
                    PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                }

                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackRightEighthToOrthogonalUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 48)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 52)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 56)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 18, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 60)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 49)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 53)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 57)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 61)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 50)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 54)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 58)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 62)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 51)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 55)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 59)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE + 63)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 1)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction + 1, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(PaintSegment::centre, PaintSegment::topLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackLeftEighthToDiagDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        uint8_t map[5] = { 4, 3, 1, 2, 0 };
        trackSequence = map[trackSequence];
        TrackRightEighthToOrthogonalUp25(session, ride, trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void TrackRightEighthToDiagDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        uint8_t map[5] = { 4, 3, 1, 2, 0 };
        trackSequence = map[trackSequence];
        TrackLeftEighthToOrthogonalUp25(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackLeftEighthToOrthogonalDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
        TrackRightEighthToDiagUp25(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackRightEighthToOrthogonalDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
        TrackLeftEighthToDiagUp25(session, ride, trackSequence, (direction + 3) & 3, height, trackElement, supportType);
    }

    static void TrackDiagUp25ToLeftBankedUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 4)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 0)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 1)),
                            { -16, -16, height }, { { -16, -16, height + 48 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 3)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 9, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 2)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 9, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 9, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 9, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackDiagUp25ToRightBankedUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 9)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 5)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 7)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 8)),
                            { -16, -16, height }, { { -16, -16, height + 48 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 9, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 6)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 9, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 9, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 9, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackDiagLeftBankedUp25ToUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 14)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 10)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 11)),
                            { -16, -16, height }, { { -16, -16, height + 48 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 13)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 9, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 12)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 9, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 9, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 9, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void TrackDiagRightBankedUp25ToUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 19)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 15)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 17)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 18)),
                            { -16, -16, height }, { { -16, -16, height + 48 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 9, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 16)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 9, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 9, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 9, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void TrackDiagDown25ToLeftBankedDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackDiagRightBankedUp25ToUp25(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackDiagDown25ToRightBankedDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackDiagLeftBankedUp25ToUp25(session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackDiagLeftBankedDown25ToDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackDiagUp25ToRightBankedUp25(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackDiagRightBankedDown25ToDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackDiagUp25ToLeftBankedUp25(session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackDiagLeftBankedFlatToLeftBankedUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 23)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 20)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 22)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 21)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackDiagRightBankedFlatToRightBankedUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 27)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 24)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 26)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 25)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackDiagLeftBankedUp25ToLeftBankedFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 31)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 28)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 30)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 6, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 29)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 6, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 6, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 6, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void TrackDiagRightBankedUp25ToRightBankedFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 35)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 32)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 34)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 6, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 33)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 6, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 6, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 6, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void TrackDiagLeftBankedFlatToLeftBankedDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackDiagRightBankedUp25ToRightBankedFlat(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackDiagRightBankedFlatToRightBankedDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackDiagLeftBankedUp25ToLeftBankedFlat(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackDiagLeftBankedDown25ToLeftBankedFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackDiagRightBankedFlatToRightBankedUp25(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackDiagRightBankedDown25ToRightBankedFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackDiagLeftBankedFlatToLeftBankedUp25(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackDiagUp25LeftBanked(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 39)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 36)),
                            { -16, -16, height }, { { -16, -16, height + 48 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 38)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 11, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 37)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 11, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 11, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 11, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void TrackDiagUp25RightBanked(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 43)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 40)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 42)),
                            { -16, -16, height }, { { -16, -16, height + 48 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 11, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 41)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 11, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 11, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 11, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void TrackDiagDown25LeftBanked(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackDiagUp25RightBanked(session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackDiagDown25RightBanked(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackDiagUp25LeftBanked(session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackDiagFlatToLeftBankedUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 48)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 44)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 45)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 47)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 46)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackDiagFlatToRightBankedUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 53)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 49)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 51)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 52)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 50)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void TrackDiagLeftBankedUp25ToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 58)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 54)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 55)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 57)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 6, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 56)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 6, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 6, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 6, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void TrackDiagRightBankedUp25ToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 63)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 59)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 61)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 62)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 6, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 60)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 6, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 6, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 6, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void TrackDiagFlatToLeftBankedDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackDiagRightBankedUp25ToFlat(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackDiagFlatToRightBankedDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackDiagLeftBankedUp25ToFlat(session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackDiagLeftBankedDown25ToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackDiagFlatToRightBankedUp25(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackDiagRightBankedDown25ToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackDiagFlatToLeftBankedUp25(session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackLeftEighthBankToDiagUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 64)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 68)),
                            { 0, 0, height }, { { 0, 31, height }, { 32, 1, 32 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 72)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 76)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 65)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 69)),
                            { 0, 0, height }, { { 0, 31, height }, { 32, 1, 32 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 73)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 77)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 66)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 70)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 40, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 74)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 78)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 67)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 71)),
                            { 0, 0, height }, { { 0, 0, height + 48 }, { 32, 32, 1 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 75)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 79)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackRightEighthBankToDiagUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 80)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 84)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 88)),
                            { 0, 0, height }, { { 0, 31, height }, { 32, 1, 32 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 92)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction, height - 8, TunnelGroup::Standard, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 81)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 85)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 89)),
                            { 0, 0, height }, { { 0, 31, height }, { 32, 1, 32 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 93)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 82)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 86)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 90)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 40, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 94)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 83)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 87)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 91)),
                            { 0, 0, height }, { { 0, 0, height + 48 }, { 32, 32, 1 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 95)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackLeftEighthBankToOrthogonalUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 96)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 32, 1 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 100)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 32, 1 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 104)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 108)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 97)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 32, 1 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 101)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 32, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 105)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 109)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 98)),
                            { 0, 0, height }, { { 0, 31, height }, { 32, 1, 32 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 102)),
                            { 0, 0, height }, { { 0, 31, height }, { 32, 1, 32 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 106)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 110)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 99)),
                            { 0, 0, height }, { { 0, 31, height }, { 32, 1, 32 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 103)),
                            { 0, 0, height }, { { 0, 31, height }, { 32, 1, 32 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 107)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 111)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 1 || direction == 2)
                {
                    PaintUtilPushTunnelRotated(session, direction, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackRightEighthBankToOrthogonalUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 112)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 116)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 32, 1 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 120)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 32, 1 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 124)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 113)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 117)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 32, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 121)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 32, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 125)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 114)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 118)),
                            { 0, 0, height }, { { 31, 0, height }, { 1, 32, 32 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 122)),
                            { 0, 0, height }, { { 31, 0, height }, { 1, 32, 32 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 126)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 115)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 119)),
                            { 0, 0, height }, { { 31, 0, height }, { 1, 32, 32 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 123)),
                            { 0, 0, height }, { { 31, 0, height }, { 1, 32, 32 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_SINGLE_RAIL_TRACK_GENTLE_LARGE_CURVE_BANKED + 127)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 1)
                {
                    PaintUtilPushTunnelRotated(
                        session, direction + 1, height + 8, TunnelGroup::Standard, TunnelSubType::SlopeEnd);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(PaintSegment::centre, PaintSegment::topLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void TrackLeftEighthBankToDiagDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        uint8_t map[5] = { 4, 3, 1, 2, 0 };
        trackSequence = map[trackSequence];
        TrackRightEighthBankToOrthogonalUp25(
            session, ride, trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void TrackRightEighthBankToDiagDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        uint8_t map[5] = { 4, 3, 1, 2, 0 };
        trackSequence = map[trackSequence];
        TrackLeftEighthBankToOrthogonalUp25(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackLeftEighthBankToOrthogonalDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
        TrackRightEighthBankToDiagUp25(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void TrackRightEighthBankToOrthogonalDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
        TrackLeftEighthBankToDiagUp25(session, ride, trackSequence, (direction + 3) & 3, height, trackElement, supportType);
    }

    TRACK_PAINT_FUNCTION GetTrackPaintFunction(int32_t trackType)
    {
        switch (trackType)
        {
            case TrackElemType::Flat:
                return TrackFlat;
            case TrackElemType::EndStation:
            case TrackElemType::BeginStation:
            case TrackElemType::MiddleStation:
                return TrackStation;
            case TrackElemType::Up25:
                return Track25DegUp;
            case TrackElemType::Up60:
                return Track60DegUp;
            case TrackElemType::FlatToUp25:
                return TrackFlatTo25DegUp;
            case TrackElemType::Up25ToUp60:
                return Track25DegUpTo60DegUp;
            case TrackElemType::Up60ToUp25:
                return Track60DegUpTo25DegUp;
            case TrackElemType::Up25ToFlat:
                return Track25DegUpToFlat;
            case TrackElemType::Down25:
                return Track25DegDown;
            case TrackElemType::Down60:
                return Track60DegDown;
            case TrackElemType::FlatToDown25:
                return TrackFlatTo25DegDown;
            case TrackElemType::Down25ToDown60:
                return Track25DegDownTo60DegDown;
            case TrackElemType::Down60ToDown25:
                return Track60DegDownTo25DegDown;
            case TrackElemType::Down25ToFlat:
                return Track25DegDownToFlat;
            case TrackElemType::Up90:
                return Track90DegUp;
            case TrackElemType::Down90:
                return Track90DegDown;
            case TrackElemType::Up60ToUp90:
                return Track60DegUpTo90DegUp;
            case TrackElemType::Down90ToDown60:
                return Track90DegDownTo60DegDown;
            case TrackElemType::Up90ToUp60:
                return Track90DegUpTo60DegUp;
            case TrackElemType::Down60ToDown90:
                return Track60DegDownTo90DegDown;
            case TrackElemType::LeftQuarterTurn5Tiles:
                return TrackLeftQuarterTurn5;
            case TrackElemType::RightQuarterTurn5Tiles:
                return TrackRightQuarterTurn5;
            case TrackElemType::FlatToLeftBank:
                return TrackFlatToLeftBank;
            case TrackElemType::FlatToRightBank:
                return TrackFlatToRightBank;
            case TrackElemType::LeftBankToFlat:
                return TrackLeftBankToFlat;
            case TrackElemType::RightBankToFlat:
                return TrackRightBankToFlat;
            case TrackElemType::BankedLeftQuarterTurn5Tiles:
                return TrackBankedLeftQuarterTurn5;
            case TrackElemType::BankedRightQuarterTurn5Tiles:
                return TrackBankedRightQuarterTurn5;
            case TrackElemType::LeftBankToUp25:
                return TrackLeftBankTo25DegUp;
            case TrackElemType::RightBankToUp25:
                return TrackRightBankTo25DegUp;
            case TrackElemType::Up25ToLeftBank:
                return Track25DegUpToLeftBank;
            case TrackElemType::Up25ToRightBank:
                return Track25DegUpToRightBank;
            case TrackElemType::LeftBankToDown25:
                return TrackLeftBankTo25DegDown;
            case TrackElemType::RightBankToDown25:
                return TrackRightBankTo25DegDown;
            case TrackElemType::Down25ToLeftBank:
                return Track25DegDownToLeftBank;
            case TrackElemType::Down25ToRightBank:
                return Track25DegDownToRightBank;
            case TrackElemType::LeftBank:
                return TrackLeftBank;
            case TrackElemType::RightBank:
                return TrackRightBank;
            case TrackElemType::LeftQuarterTurn5TilesUp25:
                return TrackLeftQuarterTurn525DegUp;
            case TrackElemType::RightQuarterTurn5TilesUp25:
                return TrackRightQuarterTurn525DegUp;
            case TrackElemType::LeftQuarterTurn5TilesDown25:
                return TrackLeftQuarterTurn525DegDown;
            case TrackElemType::RightQuarterTurn5TilesDown25:
                return TrackRightQuarterTurn525DegDown;
            case TrackElemType::SBendLeft:
                return TrackSBendLeft;
            case TrackElemType::SBendRight:
                return TrackSBendRight;
            case TrackElemType::LeftQuarterTurn3Tiles:
                return TrackLeftQuarterTurn3;
            case TrackElemType::RightQuarterTurn3Tiles:
                return TrackRightQuarterTurn3;
            case TrackElemType::LeftBankedQuarterTurn3Tiles:
                return TrackLeftQuarterTurn3Bank;
            case TrackElemType::RightBankedQuarterTurn3Tiles:
                return TrackRightQuarterTurn3Bank;
            case TrackElemType::LeftQuarterTurn3TilesUp25:
                return TrackLeftQuarterTurn3Tile25DegUp;
            case TrackElemType::RightQuarterTurn3TilesUp25:
                return TrackRightQuarterTurn3Tile25DegUp;
            case TrackElemType::LeftQuarterTurn3TilesDown25:
                return TrackLeftQuarterTurn3Tile25DegDown;
            case TrackElemType::RightQuarterTurn3TilesDown25:
                return TrackRightQuarterTurn3Tile25DegDown;
            case TrackElemType::LeftHalfBankedHelixUpSmall:
                return TrackLeftHalfBankedHelixUpSmall;
            case TrackElemType::RightHalfBankedHelixUpSmall:
                return TrackRightHalfBankedHelixUpSmall;
            case TrackElemType::LeftHalfBankedHelixDownSmall:
                return TrackLeftHalfBankedHelixDownSmall;
            case TrackElemType::RightHalfBankedHelixDownSmall:
                return TrackRightHalfBankedHelixDownSmall;
            case TrackElemType::LeftHalfBankedHelixUpLarge:
                return TrackLeftHalfBankedHelixUpLarge;
            case TrackElemType::RightHalfBankedHelixUpLarge:
                return TrackRightHalfBankedHelixUpLarge;
            case TrackElemType::LeftHalfBankedHelixDownLarge:
                return TrackLeftHalfBankedHelixDownLarge;
            case TrackElemType::RightHalfBankedHelixDownLarge:
                return TrackRightHalfBankedHelixDownLarge;
            case TrackElemType::LeftQuarterTurn1TileUp60:
                return TrackLeftQuarterTurn160DegUp;
            case TrackElemType::RightQuarterTurn1TileUp60:
                return TrackRightQuarterTurn160DegUp;
            case TrackElemType::LeftQuarterTurn1TileDown60:
                return TrackLeftQuarterTurn160DegDown;
            case TrackElemType::RightQuarterTurn1TileDown60:
                return TrackRightQuarterTurn160DegDown;
            case TrackElemType::Brakes:
                return TrackBrakes;
            case TrackElemType::Up25LeftBanked:
                return Track25DegUpLeftBanked;
            case TrackElemType::Up25RightBanked:
                return Track25DegUpRightBanked;
            case TrackElemType::OnRidePhoto:
                return TrackOnRidePhoto;
            case TrackElemType::Down25LeftBanked:
                return Track25DegDownLeftBanked;
            case TrackElemType::Down25RightBanked:
                return Track25DegDownRightBanked;
            case TrackElemType::FlatToUp60LongBase:
                return TrackFlatTo60DegUpLongBase;
            case TrackElemType::Up60ToFlatLongBase:
                return Track60DegUpToFlatLongBase;
            case TrackElemType::FlatToDown60LongBase:
                return TrackFlatTo60DegDownLongBase;
            case TrackElemType::Down60ToFlatLongBase:
                return Track60DegDownToFlatLongBase;
            case TrackElemType::LeftEighthToDiag:
                return TrackLeftEighthToDiag;
            case TrackElemType::RightEighthToDiag:
                return TrackRightEighthToDiag;
            case TrackElemType::LeftEighthToOrthogonal:
                return TrackLeftEighthToOrthogonal;
            case TrackElemType::RightEighthToOrthogonal:
                return TrackRightEighthToOrthogonal;
            case TrackElemType::LeftEighthBankToDiag:
                return TrackLeftEighthBankToDiag;
            case TrackElemType::RightEighthBankToDiag:
                return TrackRightEighthBankToDiag;
            case TrackElemType::LeftEighthBankToOrthogonal:
                return TrackLeftEighthBankToOrthogonal;
            case TrackElemType::RightEighthBankToOrthogonal:
                return TrackRightEighthBankToOrthogonal;
            case TrackElemType::DiagFlat:
                return TrackDiagFlat;
            case TrackElemType::DiagBrakes:
                return TrackDiagBrakes;
            case TrackElemType::DiagBlockBrakes:
                return TrackDiagBlockBrakes;
            case TrackElemType::DiagUp25:
                return TrackDiag25DegUp;
            case TrackElemType::DiagUp60:
                return TrackDiag60DegUp;
            case TrackElemType::DiagFlatToUp25:
                return TrackDiagFlatTo25DegUp;
            case TrackElemType::DiagUp25ToUp60:
                return TrackDiag25DegUpTo60DegUp;
            case TrackElemType::DiagUp60ToUp25:
                return TrackDiag60DegUpTo25DegUp;
            case TrackElemType::DiagUp25ToFlat:
                return TrackDiag25DegUpToFlat;
            case TrackElemType::DiagDown25:
                return TrackDiag25DegDown;
            case TrackElemType::DiagDown60:
                return TrackDiag60DegDown;
            case TrackElemType::DiagFlatToDown25:
                return TrackDiagFlatTo25DegDown;
            case TrackElemType::DiagDown25ToDown60:
                return TrackDiag25DegDownTo60DegDown;
            case TrackElemType::DiagDown60ToDown25:
                return TrackDiag60DegDownTo25DegDown;
            case TrackElemType::DiagDown25ToFlat:
                return TrackDiag25DegDownToFlat;
            case TrackElemType::DiagFlatToLeftBank:
                return TrackDiagFlatToLeftBank;
            case TrackElemType::DiagFlatToRightBank:
                return TrackDiagFlatToRightBank;
            case TrackElemType::DiagLeftBankToFlat:
                return TrackDiagLeftBankToFlat;
            case TrackElemType::DiagRightBankToFlat:
                return TrackDiagRightBankToFlat;
            case TrackElemType::DiagLeftBankToUp25:
                return TrackDiagLeftBankTo25DegUp;
            case TrackElemType::DiagRightBankToUp25:
                return TrackDiagRightBankTo25DegUp;
            case TrackElemType::DiagUp25ToLeftBank:
                return TrackDiag25DegUpToLeftBank;
            case TrackElemType::DiagUp25ToRightBank:
                return TrackDiag25DegUpToRightBank;
            case TrackElemType::DiagLeftBankToDown25:
                return TrackDiagLeftBankTo25DegDown;
            case TrackElemType::DiagRightBankToDown25:
                return TrackDiagRightBankTo25DegDown;
            case TrackElemType::DiagDown25ToLeftBank:
                return TrackDiag25DegDownToLeftBank;
            case TrackElemType::DiagDown25ToRightBank:
                return TrackDiag25DegDownToRightBank;
            case TrackElemType::DiagLeftBank:
                return TrackDiagLeftBank;
            case TrackElemType::DiagRightBank:
                return TrackDiagRightBank;
            case TrackElemType::BlockBrakes:
                return TrackBlockBrakes;
            case TrackElemType::LeftBankedQuarterTurn3TileUp25:
                return TrackLeftBankedQuarterTurn3Tile25DegUp;
            case TrackElemType::RightBankedQuarterTurn3TileUp25:
                return TrackRightBankedQuarterTurn3Tile25DegUp;
            case TrackElemType::LeftBankedQuarterTurn3TileDown25:
                return TrackLeftBankedQuarterTurn3Tile25DegDown;
            case TrackElemType::RightBankedQuarterTurn3TileDown25:
                return TrackRightBankedQuarterTurn3Tile25DegDown;
            case TrackElemType::LeftBankedQuarterTurn5TileUp25:
                return TrackLeftBankedQuarterTurn525DegUp;
            case TrackElemType::RightBankedQuarterTurn5TileUp25:
                return TrackRightBankedQuarterTurn525DegUp;
            case TrackElemType::LeftBankedQuarterTurn5TileDown25:
                return TrackLeftBankedQuarterTurn525DegDown;
            case TrackElemType::RightBankedQuarterTurn5TileDown25:
                return TrackRightBankedQuarterTurn525DegDown;
            case TrackElemType::Up25ToLeftBankedUp25:
                return Track25DegUpToLeftBanked25DegUp;
            case TrackElemType::Up25ToRightBankedUp25:
                return Track25DegUpToRightBanked25DegUp;
            case TrackElemType::LeftBankedUp25ToUp25:
                return TrackLeftBanked25DegUpTo25DegUp;
            case TrackElemType::RightBankedUp25ToUp25:
                return TrackRightBanked25DegUpTo25DegUp;
            case TrackElemType::Down25ToLeftBankedDown25:
                return Track25DegDownToLeftBanked25DegDown;
            case TrackElemType::Down25ToRightBankedDown25:
                return Track25DegDownToRightBanked25DegDown;
            case TrackElemType::LeftBankedDown25ToDown25:
                return TrackLeftBanked25DegDownTo25DegDown;
            case TrackElemType::RightBankedDown25ToDown25:
                return TrackRightBanked25DegDownTo25DegDown;
            case TrackElemType::LeftBankedFlatToLeftBankedUp25:
                return TrackLeftBankedFlatToLeftBanked25DegUp;
            case TrackElemType::RightBankedFlatToRightBankedUp25:
                return TrackRightBankedFlatToRightBanked25DegUp;
            case TrackElemType::LeftBankedUp25ToLeftBankedFlat:
                return TrackLeftBanked25DegUpToLeftBankedFlat;
            case TrackElemType::RightBankedUp25ToRightBankedFlat:
                return TrackRightBanked25DegUpToRightBankedFlat;
            case TrackElemType::LeftBankedFlatToLeftBankedDown25:
                return TrackLeftBankedFlatToLeftBanked25DegDown;
            case TrackElemType::RightBankedFlatToRightBankedDown25:
                return TrackRightBankedFlatToRightBanked25DegDown;
            case TrackElemType::LeftBankedDown25ToLeftBankedFlat:
                return TrackLeftBanked25DegDownToLeftBankedFlat;
            case TrackElemType::RightBankedDown25ToRightBankedFlat:
                return TrackRightBanked25DegDownToRightBankedFlat;
            case TrackElemType::FlatToLeftBankedUp25:
                return TrackFlatToLeftBanked25DegUp;
            case TrackElemType::FlatToRightBankedUp25:
                return TrackFlatToRightBanked25DegUp;
            case TrackElemType::LeftBankedUp25ToFlat:
                return TrackLeftBanked25DegUpToFlat;
            case TrackElemType::RightBankedUp25ToFlat:
                return TrackRightBanked25DegUpToFlat;
            case TrackElemType::FlatToLeftBankedDown25:
                return TrackFlatToLeftBanked25DegDown;
            case TrackElemType::FlatToRightBankedDown25:
                return TrackFlatToRightBanked25DegDown;
            case TrackElemType::LeftBankedDown25ToFlat:
                return TrackLeftBanked25DegDownToFlat;
            case TrackElemType::RightBankedDown25ToFlat:
                return TrackRightBanked25DegDownToFlat;
            case TrackElemType::LeftQuarterTurn1TileUp90:
                return TrackLeftQuarterTurn190DegUp;
            case TrackElemType::RightQuarterTurn1TileUp90:
                return TrackRightQuarterTurn190DegUp;
            case TrackElemType::LeftQuarterTurn1TileDown90:
                return TrackLeftQuarterTurn190DegDown;
            case TrackElemType::RightQuarterTurn1TileDown90:
                return TrackRightQuarterTurn190DegDown;
            case TrackElemType::LeftBarrelRollUpToDown:
                return TrackLeftBarrelRollUpToDown;
            case TrackElemType::RightBarrelRollUpToDown:
                return TrackRightBarrelRollUpToDown;
            case TrackElemType::LeftBarrelRollDownToUp:
                return TrackLeftBarrelRollDownToUp;
            case TrackElemType::RightBarrelRollDownToUp:
                return TrackRightBarrelRollDownToUp;
            case TrackElemType::HalfLoopUp:
                return TrackHalfLoopUp;
            case TrackElemType::HalfLoopDown:
                return TrackHalfLoopDown;
            case TrackElemType::LeftCorkscrewUp:
                return TrackLeftCorkscrewUp;
            case TrackElemType::RightCorkscrewUp:
                return TrackRightCorkscrewUp;
            case TrackElemType::LeftCorkscrewDown:
                return TrackLeftCorkscrewDown;
            case TrackElemType::RightCorkscrewDown:
                return TrackRightCorkscrewDown;
            case TrackElemType::LeftLargeCorkscrewUp:
                return TrackLeftLargeCorkscrewUp;
            case TrackElemType::RightLargeCorkscrewUp:
                return TrackRightLargeCorkscrewUp;
            case TrackElemType::LeftLargeCorkscrewDown:
                return TrackLeftLargeCorkscrewDown;
            case TrackElemType::RightLargeCorkscrewDown:
                return TrackRightLargeCorkscrewDown;
            case TrackElemType::LeftZeroGRollUp:
                return TrackLeftZeroGRollUp;
            case TrackElemType::RightZeroGRollUp:
                return TrackRightZeroGRollUp;
            case TrackElemType::LeftZeroGRollDown:
                return TrackLeftZeroGRollDown;
            case TrackElemType::RightZeroGRollDown:
                return TrackRightZeroGRollDown;
            case TrackElemType::LeftLargeZeroGRollUp:
                return TrackLeftLargeZeroGRollUp;
            case TrackElemType::RightLargeZeroGRollUp:
                return TrackRightLargeZeroGRollUp;
            case TrackElemType::LeftLargeZeroGRollDown:
                return TrackLeftLargeZeroGRollDown;
            case TrackElemType::RightLargeZeroGRollDown:
                return TrackRightLargeZeroGRollDown;
            case TrackElemType::Up90ToInvertedFlatQuarterLoop:
                return Track90DegToInvertedFlatQuarterLoopUp;
            case TrackElemType::InvertedFlatToDown90QuarterLoop:
                return TrackInvertedFlatTo90DegQuarterLoopDown;
            case TrackElemType::LeftBankToLeftQuarterTurn3TilesUp25:
                return TrackLeftBankToLeftQuarterTurn3Tile25DegUp;
            case TrackElemType::RightBankToRightQuarterTurn3TilesUp25:
                return TrackRightBankToRightQuarterTurn3Tile25DegUp;
            case TrackElemType::LeftQuarterTurn3TilesDown25ToLeftBank:
                return TrackLeftQuarterTurn3Tile25DegDownToLeftBank;
            case TrackElemType::RightQuarterTurn3TilesDown25ToRightBank:
                return TrackRightQuarterTurn3Tile25DegDownToRightBank;
            case TrackElemType::LeftMediumHalfLoopUp:
                return TrackLeftMediumHalfLoopUp;
            case TrackElemType::RightMediumHalfLoopUp:
                return TrackRightMediumHalfLoopUp;
            case TrackElemType::LeftMediumHalfLoopDown:
                return TrackLeftMediumHalfLoopDown;
            case TrackElemType::RightMediumHalfLoopDown:
                return TrackRightMediumHalfLoopDown;
            case TrackElemType::LeftLargeHalfLoopUp:
                return TrackLeftLargeHalfLoopUp;
            case TrackElemType::RightLargeHalfLoopUp:
                return TrackRightLargeHalfLoopUp;
            case TrackElemType::LeftLargeHalfLoopDown:
                return TrackLeftLargeHalfLoopDown;
            case TrackElemType::RightLargeHalfLoopDown:
                return TrackRightLargeHalfLoopDown;
            case TrackElemType::FlatToUp60:
                return TrackFlatTo60DegUp;
            case TrackElemType::Up60ToFlat:
                return Track60DegUpToFlat;
            case TrackElemType::FlatToDown60:
                return TrackFlatTo60DegDown;
            case TrackElemType::Down60ToFlat:
                return Track60DegDownToFlat;
            case TrackElemType::DiagFlatToUp60:
                return TrackDiagFlatTo60DegUp;
            case TrackElemType::DiagUp60ToFlat:
                return TrackDiag60DegUpToFlat;
            case TrackElemType::DiagFlatToDown60:
                return TrackDiagFlatTo60DegDown;
            case TrackElemType::DiagDown60ToFlat:
                return TrackDiag60DegDownToFlat;

            case TrackElemType::LeftEighthToDiagUp25:
                return TrackLeftEighthToDiagUp25;
            case TrackElemType::RightEighthToDiagUp25:
                return TrackRightEighthToDiagUp25;
            case TrackElemType::LeftEighthToDiagDown25:
                return TrackLeftEighthToDiagDown25;
            case TrackElemType::RightEighthToDiagDown25:
                return TrackRightEighthToDiagDown25;
            case TrackElemType::LeftEighthToOrthogonalUp25:
                return TrackLeftEighthToOrthogonalUp25;
            case TrackElemType::RightEighthToOrthogonalUp25:
                return TrackRightEighthToOrthogonalUp25;
            case TrackElemType::LeftEighthToOrthogonalDown25:
                return TrackLeftEighthToOrthogonalDown25;
            case TrackElemType::RightEighthToOrthogonalDown25:
                return TrackRightEighthToOrthogonalDown25;

            case TrackElemType::DiagUp25ToLeftBankedUp25:
                return TrackDiagUp25ToLeftBankedUp25;
            case TrackElemType::DiagUp25ToRightBankedUp25:
                return TrackDiagUp25ToRightBankedUp25;
            case TrackElemType::DiagLeftBankedUp25ToUp25:
                return TrackDiagLeftBankedUp25ToUp25;
            case TrackElemType::DiagRightBankedUp25ToUp25:
                return TrackDiagRightBankedUp25ToUp25;
            case TrackElemType::DiagDown25ToLeftBankedDown25:
                return TrackDiagDown25ToLeftBankedDown25;
            case TrackElemType::DiagDown25ToRightBankedDown25:
                return TrackDiagDown25ToRightBankedDown25;
            case TrackElemType::DiagLeftBankedDown25ToDown25:
                return TrackDiagLeftBankedDown25ToDown25;
            case TrackElemType::DiagRightBankedDown25ToDown25:
                return TrackDiagRightBankedDown25ToDown25;

            case TrackElemType::DiagLeftBankedFlatToLeftBankedUp25:
                return TrackDiagLeftBankedFlatToLeftBankedUp25;
            case TrackElemType::DiagRightBankedFlatToRightBankedUp25:
                return TrackDiagRightBankedFlatToRightBankedUp25;
            case TrackElemType::DiagLeftBankedUp25ToLeftBankedFlat:
                return TrackDiagLeftBankedUp25ToLeftBankedFlat;
            case TrackElemType::DiagRightBankedUp25ToRightBankedFlat:
                return TrackDiagRightBankedUp25ToRightBankedFlat;
            case TrackElemType::DiagLeftBankedFlatToLeftBankedDown25:
                return TrackDiagLeftBankedFlatToLeftBankedDown25;
            case TrackElemType::DiagRightBankedFlatToRightBankedDown25:
                return TrackDiagRightBankedFlatToRightBankedDown25;
            case TrackElemType::DiagLeftBankedDown25ToLeftBankedFlat:
                return TrackDiagLeftBankedDown25ToLeftBankedFlat;
            case TrackElemType::DiagRightBankedDown25ToRightBankedFlat:
                return TrackDiagRightBankedDown25ToRightBankedFlat;
            case TrackElemType::DiagUp25LeftBanked:
                return TrackDiagUp25LeftBanked;
            case TrackElemType::DiagUp25RightBanked:
                return TrackDiagUp25RightBanked;
            case TrackElemType::DiagDown25LeftBanked:
                return TrackDiagDown25LeftBanked;
            case TrackElemType::DiagDown25RightBanked:
                return TrackDiagDown25RightBanked;
            case TrackElemType::DiagFlatToLeftBankedUp25:
                return TrackDiagFlatToLeftBankedUp25;
            case TrackElemType::DiagFlatToRightBankedUp25:
                return TrackDiagFlatToRightBankedUp25;
            case TrackElemType::DiagLeftBankedUp25ToFlat:
                return TrackDiagLeftBankedUp25ToFlat;
            case TrackElemType::DiagRightBankedUp25ToFlat:
                return TrackDiagRightBankedUp25ToFlat;
            case TrackElemType::DiagFlatToLeftBankedDown25:
                return TrackDiagFlatToLeftBankedDown25;
            case TrackElemType::DiagFlatToRightBankedDown25:
                return TrackDiagFlatToRightBankedDown25;
            case TrackElemType::DiagLeftBankedDown25ToFlat:
                return TrackDiagLeftBankedDown25ToFlat;
            case TrackElemType::DiagRightBankedDown25ToFlat:
                return TrackDiagRightBankedDown25ToFlat;
            case TrackElemType::LeftEighthBankToDiagUp25:
                return TrackLeftEighthBankToDiagUp25;
            case TrackElemType::RightEighthBankToDiagUp25:
                return TrackRightEighthBankToDiagUp25;
            case TrackElemType::LeftEighthBankToDiagDown25:
                return TrackLeftEighthBankToDiagDown25;
            case TrackElemType::RightEighthBankToDiagDown25:
                return TrackRightEighthBankToDiagDown25;
            case TrackElemType::LeftEighthBankToOrthogonalUp25:
                return TrackLeftEighthBankToOrthogonalUp25;
            case TrackElemType::RightEighthBankToOrthogonalUp25:
                return TrackRightEighthBankToOrthogonalUp25;
            case TrackElemType::LeftEighthBankToOrthogonalDown25:
                return TrackLeftEighthBankToOrthogonalDown25;
            case TrackElemType::RightEighthBankToOrthogonalDown25:
                return TrackRightEighthBankToOrthogonalDown25;
        }
        return nullptr;
    }

} // namespace OpenRCT2::SingleRailRC