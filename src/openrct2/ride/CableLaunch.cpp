/*****************************************************************************
 * Copyright (c) 2014-2023 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "CableLaunch.h"

#include "../audio/audio.h"
#include "../entity/EntityList.h"
#include "../rct12/RCT12.h"
#include "../util/Util.h"
#include "Ride.h"
#include "RideData.h"
#include "Track.h"
#include "Vehicle.h"
#include "VehicleData.h"

#include <algorithm>

Vehicle* CableLaunchSegmentCreate(
    Ride& ride, int32_t x, int32_t y, int32_t z, int32_t direction, uint16_t var_44, int32_t remaining_distance, int index)
{
    Vehicle* current = CreateEntity<Vehicle>();
    current->ride = ride.id;
    current->ride_subtype = OBJECT_ENTRY_INDEX_NULL;
    if (index == 0)
    {
        ride.cable_lift = current->Id;
    }
    current->SubType = index==0 ? Vehicle::Type::Head : Vehicle::Type::Tail;
    current->var_44 = var_44;
    current->remaining_distance = remaining_distance;
    current->SpriteData.Width = 10;
    current->SpriteData.HeightMin = 10;
    current->SpriteData.HeightMax = 10;
    current->mass = 100;
    current->num_seats = 0;
    current->speed = 20;
    current->powered_acceleration = 80;
    current->velocity = 0;
    current->acceleration = 0;
    current->SwingSprite = 0;
    current->SwingPosition = 0;
    current->SwingSpeed = 0;
    current->restraints_position = 0;
    current->spin_sprite = 0;
    current->spin_speed = 0;
    current->sound2_flags = 0;
    current->sound1_id = OpenRCT2::Audio::SoundId::Null;
    current->sound2_id = OpenRCT2::Audio::SoundId::Null;
    current->CollisionDetectionTimer = 0;
    current->animation_frame = 0;
    current->vehicle_type = index;
    current->animationState = 0;
    current->scream_sound_id = OpenRCT2::Audio::SoundId::Null;
    current->Pitch = 0;
    current->bank_rotation = 0;

    for (auto& peep : current->peep)
    {
        peep = EntityId::GetNull();
    }
    current->TrackSubposition = VehicleTrackSubposition::Default;
    current->Orientation = direction << 3;

    z = z * COORDS_Z_STEP;
    current->TrackLocation = { x, y, z };
    z += ride.GetRideTypeDescriptor().Heights.VehicleZOffset;

    current->MoveTo({ 16, 16, z });
    current->SetTrackType(TrackElemType::CableLaunch);
    current->SetTrackDirection(current->Orientation >> 3);
    current->track_progress = 80;
    current->Flags = VehicleFlags::CollisionDisabled | VehicleFlags::CarIsCableLaunch;
    current->SetState(Vehicle::Status::MovingToEndOfStation, 0);
    current->num_peeps = 0;
    current->next_free_seat = 0;
    current->BoatLocation.SetNull();
    return current;
}


void Vehicle::CableLaunchUpdate()
{
    switch (status)
    {
        case Vehicle::Status::MovingToEndOfStation:
            CableLaunchUpdateMovingToEndOfStation();
            break;
        case Vehicle::Status::WaitingForPassengers:
            // Stays in this state until a train puts it into next state
            break;
        case Vehicle::Status::WaitingToDepart:
            CableLaunchUpdateWaitingToDepart();
            break;
        case Vehicle::Status::Departing:
            CableLaunchUpdateDeparting();
            break;
        case Vehicle::Status::Travelling:
            CableLaunchUpdateTravelling();
            break;
        case Vehicle::Status::Arriving:
            CableLaunchUpdateArriving();
            break;
    //    case Vehicle::Status::PreparingForCableLaunch:
    //        CableLaunchUpdatePreparingForCableLaunch();
            break;
        default:
            break;
    }
}

void Vehicle::CableLaunchUpdateMovingToEndOfStation()
{
    if (velocity >= -300000)//TODO determine final speed
    {
        acceleration = -2932;
    }

    if (velocity < -300000)
    {
        velocity -= velocity / 16;
        acceleration = 0;
    }

    if (!(CableLaunchUpdateTrackMotion() & VEHICLE_UPDATE_MOTION_TRACK_FLAG_VEHICLE_AT_STATION))
        return;

SetState(Vehicle::Status::Arriving, sub_state);
}

void Vehicle::CableLaunchUpdateArriving()
{

//Slow catch car down
    if (velocity < -50000)
    {
    acceleration = 10000;
    }

//Continue moving until it is in the right position

CableLiftUpdateTrackMotion();

   if(track_progress==3)
   {
   velocity=0;
   acceleration=0;
   SetState(Vehicle::Status::WaitingForPassengers, sub_state);
   }
//    sub_state++;
//    if (sub_state >= 64)
//        SetState(Vehicle::Status::MovingToEndOfStation, sub_state);
}

static void CableLaunchUpdateFinStateForTile(CoordsXYZ loc,TrackElement* tileElement,bool set_lowered)
{
       uint8_t state=tileElement->GetCableLaunchFinState();
       uint8_t new_state=state;
       //TODO misses tiles when train is going faster than 230mph - code should not allow tiles to be set in a state lower than the tile ahead
       		switch(state)
       		{
       		case CABLE_LAUNCH_FIN_STATE_RAISING_0:
       			new_state=CABLE_LAUNCH_FIN_STATE_RAISING_1;
       		break;
       		case CABLE_LAUNCH_FIN_STATE_RAISING_1:
       			new_state=CABLE_LAUNCH_FIN_STATE_RAISING_2;
       		break;
       		case CABLE_LAUNCH_FIN_STATE_RAISING_2:
       			new_state=CABLE_LAUNCH_FIN_STATE_RAISING_3;
       		break;
       		case CABLE_LAUNCH_FIN_STATE_RAISING_3:
       			new_state=CABLE_LAUNCH_FIN_STATE_RAISED;
       		break;
       		case CABLE_LAUNCH_FIN_STATE_LOWERING_0:
       			new_state=CABLE_LAUNCH_FIN_STATE_LOWERING_1;
       		break;
       		case CABLE_LAUNCH_FIN_STATE_LOWERING_1:
       			new_state=CABLE_LAUNCH_FIN_STATE_LOWERING_2;
       		break;
       		case CABLE_LAUNCH_FIN_STATE_LOWERING_2:
       			new_state=CABLE_LAUNCH_FIN_STATE_LOWERED;
       		break;
       		default:
       		break;
       		}
       	if(set_lowered)new_state=CABLE_LAUNCH_FIN_STATE_LOWERING_0;
       	if(new_state!=state)
               {
               tileElement->SetCableLaunchFinState(new_state);
               MapInvalidateElement(loc, reinterpret_cast<TileElement*>(tileElement));
               }

}

static void CableLaunchUpdateFinState(const Ride& ride,bool set_lowered)
{
    TileElement* cableLaunchTileElement = MapGetFirstElementAt(ride.CableLiftLoc);
    if (cableLaunchTileElement == nullptr)return;
bool success=false;
    do
    {
        if (cableLaunchTileElement->GetType() != TileElementType::Track)
            continue;
        if (cableLaunchTileElement->GetBaseZ() != ride.CableLiftLoc.z)
            continue;

        success = true;
        break;
    } while (!(cableLaunchTileElement++)->IsLastForTile());

    if (!success) return;



   //First launch tile
    CableLaunchUpdateFinStateForTile(ride.CableLiftLoc, cableLaunchTileElement->AsTrack(),set_lowered);

   //Second launch tile
   auto type = cableLaunchTileElement->AsTrack()->GetTrackType();
   uint8_t rotation = cableLaunchTileElement->GetDirection();
   CoordsXY offsets = { -32, 0};
   CoordsXYZD elem = { ride.CableLiftLoc.x,ride.CableLiftLoc.y,ride.CableLiftLoc.z, rotation };
   elem += offsets.Rotate(rotation);
   TrackElement* trackElement = MapGetTrackElementAtOfTypeSeq(elem, type, 1);
           if (trackElement != nullptr)
           {
           CableLaunchUpdateFinStateForTile({elem.x,elem.y,elem.z},trackElement,set_lowered);
           //cableLaunchTileElement->AsTrack()->SetCableLaunchFinState(trackElement->GetCableLaunchFinState());
   	   }

    //Rest of the tiles
    success = false;
    TrackCircuitIterator it;
    TrackCircuitIteratorBegin(&it, { ride.CableLiftLoc, cableLaunchTileElement });
    while (TrackCircuitIteratorPrevious(&it) && !success)
    {
        TileElement* tileElement = it.current.element;
	CoordsXYZ loc= {it.current.x,it.current.y,it.currentZ};
       auto trackType = tileElement->AsTrack()->GetTrackType();
       if (tileElement->AsTrack()->HasCableLift() && (trackType == TrackElemType::Flat || trackType == TrackElemType::EndStation|| trackType == TrackElemType::BlockBrakes))CableLaunchUpdateFinStateForTile(loc,tileElement->AsTrack(),set_lowered);
	else break;
    }


}

void Vehicle::CableLaunchUpdateWaitingToDepart()
{
    auto curRide = GetRide();

    if(sub_state==0)
    {
    CableLaunchUpdateFinState(*curRide,true);
    }
    else if(sub_state%8==0)
    {
    CableLaunchUpdateFinState(*curRide,false);
    }
    sub_state++;

    if (sub_state < 40)
        return;


    velocity = 0;
    acceleration = 0;
    SetState(Vehicle::Status::Departing, 0);
}



void Vehicle::CableLaunchUpdateDeparting()
{
    sub_state++;
    if (sub_state < 16)
        return;
    

    Vehicle* passengerVehicle = GetEntity<Vehicle>(cable_lift_target);
    if (passengerVehicle == nullptr)
    {
        return;
    }
    
    SetState(Vehicle::Status::Travelling, 0);
    passengerVehicle->SetState(Vehicle::Status::TravellingCableLift, passengerVehicle->sub_state);
}

void Vehicle::CableLaunchUpdateTravelling()
{
    Vehicle* passengerVehicle = GetEntity<Vehicle>(cable_lift_target);
    if (passengerVehicle == nullptr)
    {
        return;
    }


    if(sub_state%4==0)
    {
    auto curRide = GetRide();
    CableLaunchUpdateFinState(*curRide,false);
    	if(!(sub_state&0x80))sub_state=0;
    }
    sub_state++; 


//Get rearmost vehicle in train
    Vehicle* rearVehicle=passengerVehicle;
    Vehicle* nextVehicle=GetEntity<Vehicle>(rearVehicle->next_vehicle_on_train);	
	while(nextVehicle!=nullptr)
        {
	rearVehicle=nextVehicle;
        nextVehicle=GetEntity<Vehicle>(rearVehicle->next_vehicle_on_train);	
	}




//If on the launch end piece and more than halfway through, then raise the first tile of brakes
    TileElement* trackElement = MapGetTrackElementAtOfTypeSeq(rearVehicle->TrackLocation, rearVehicle->GetTrackType(), 0);
	if(rearVehicle->GetTrackType()==TrackElemType::CableLaunch&&rearVehicle->track_progress>32)
	{
        trackElement->AsTrack()->SetCableLaunchFinState(CABLE_LAUNCH_FIN_STATE_RAISING_0);
	}


    //Get track element previous to the one it is currently on
         if(trackElement!=nullptr)
         {
         TrackBeginEnd trackBeginEnd;
             if(TrackBlockGetPrevious({rearVehicle->TrackLocation, trackElement }, &trackBeginEnd) && trackBeginEnd.begin_element!=nullptr&&trackBeginEnd.begin_element->AsTrack()->HasCableLift()&&trackBeginEnd.begin_element->AsTrack()->GetCableLaunchFinState()==CABLE_LAUNCH_FIN_STATE_LOWERED)
             {
             //TODO this seems to return the location of the first tile of the cable launch end piece, but the track element corresponding to the last tile, which is confusing
             trackBeginEnd.begin_element->AsTrack()->SetCableLaunchFinState(CABLE_LAUNCH_FIN_STATE_RAISING_0);
             }
            }


//Wait a few seconds before returning
    if((sub_state&0x7F)==127)
    {
    SetState(Vehicle::Status::MovingToEndOfStation, 0);
    return;
    }


//If the catch car has already stopped, don't run the motion update
    if(sub_state&0x80)return;


//Update motion
        if (passengerVehicle->status==Vehicle::Status::TravellingCableLift)
	{
        velocity = passengerVehicle->velocity;
        acceleration = 0;
        }
        else
        {
        int brake=std::min(velocity/8,127500);
        velocity-=brake;
		if(velocity < 16)
		{
		velocity=0;
		acceleration=0;
		sub_state|=0x80;
		}
        }

    if (passengerVehicle->HasFlag(VehicleFlags::TrainIsBroken))
        return;

//If catch car has reached the end of the launch track, stop it completely
    if (CableLaunchUpdateTrackMotion() & VEHICLE_UPDATE_MOTION_TRACK_FLAG_1)
    {
    velocity=0;
    acceleration=0;
    sub_state|=0x80;
    }
}

//TODO unify with cable lift

bool Vehicle::CableLaunchUpdateTrackMotionForwards()
{
    auto curRide = GetRide();
    if (curRide == nullptr)
        return false;

    for (; remaining_distance >= 13962; _vehicleUnkF64E10++)
    {
        auto trackType = GetTrackType();
        if (trackType == TrackElemType::CableLaunch && track_progress == 56)
        {
            _vehicleMotionTrackFlags |= VEHICLE_UPDATE_MOTION_TRACK_FLAG_1;
        }

        uint16_t trackProgress = track_progress + 1;

        uint16_t trackTotalProgress = GetTrackProgress();
        if (trackProgress >= trackTotalProgress)
        {
            TileElement* trackElement = MapGetTrackElementAtOfTypeSeq(TrackLocation, trackType, 0);

            CoordsXYE output;
            int32_t outputZ;
            int32_t outputDirection;

            auto input = CoordsXYE{ TrackLocation, trackElement };

            if (!TrackBlockGetNext(&input, &output, &outputZ, &outputDirection))
                return false;

            if (TrackPitchAndRollEnd(trackType) != TrackPitchAndRollStart(output.element->AsTrack()->GetTrackType()))
                return false;

            TrackLocation = { output, outputZ };
            SetTrackDirection(outputDirection);
            SetTrackType(output.element->AsTrack()->GetTrackType());
            brake_speed = output.element->AsTrack()->GetBrakeBoosterSpeed();
            trackProgress = 0;
        }

        track_progress = trackProgress;
        const auto moveInfo = GetMoveInfo();
        auto nextVehiclePosition = CoordsXYZ{ moveInfo->x, moveInfo->y, moveInfo->z } + TrackLocation;

        uint8_t remainingDistanceFlags = 0;
        nextVehiclePosition.z += GetRideTypeDescriptor(curRide->type).Heights.VehicleZOffset;
        if (nextVehiclePosition.x != _vehicleCurPosition.x)
            remainingDistanceFlags |= (1 << 0);
        if (nextVehiclePosition.y != _vehicleCurPosition.y)
            remainingDistanceFlags |= (1 << 1);
        if (nextVehiclePosition.z != _vehicleCurPosition.z)
            remainingDistanceFlags |= (1 << 2);

        remaining_distance -= SubpositionTranslationDistances[remainingDistanceFlags];
        _vehicleCurPosition.x = nextVehiclePosition.x;
        _vehicleCurPosition.y = nextVehiclePosition.y;
        _vehicleCurPosition.z = nextVehiclePosition.z;

        Orientation = moveInfo->direction;
        bank_rotation = moveInfo->bank_rotation;
        Pitch = moveInfo->Pitch;

        if (remaining_distance >= 13962)
        {
            acceleration += AccelerationFromPitch[Pitch];
        }
    }
    return true;
}

bool Vehicle::CableLaunchUpdateTrackMotionBackwards()
{
    auto curRide = GetRide();
    if (curRide == nullptr)
        return false;

    for (; remaining_distance < 0; _vehicleUnkF64E10++)
    {
        uint16_t trackProgress = track_progress - 1;

        if (static_cast<int16_t>(trackProgress) == -1)
        {
            auto trackType = GetTrackType();
            TileElement* trackElement = MapGetTrackElementAtOfTypeSeq(TrackLocation, trackType, 0);

            auto input = CoordsXYE{ TrackLocation, trackElement };
            TrackBeginEnd output;

            if (!TrackBlockGetPrevious(input, &output))
                return false;

            if (TrackPitchAndRollStart(trackType) != TrackPitchAndRollEnd(output.begin_element->AsTrack()->GetTrackType()))
                return false;

            TrackLocation = { output.begin_x, output.begin_y, output.begin_z };
            SetTrackDirection(output.begin_direction);
            SetTrackType(output.begin_element->AsTrack()->GetTrackType());

            if (output.begin_element->AsTrack()->GetTrackType() == TrackElemType::EndStation || output.begin_element->AsTrack()->GetTrackType() == TrackElemType::BlockBrakes)
            {
                _vehicleMotionTrackFlags = VEHICLE_UPDATE_MOTION_TRACK_FLAG_VEHICLE_AT_STATION;
            }

            uint16_t trackTotalProgress = GetTrackProgress();
            trackProgress = trackTotalProgress - 1;
        }
        track_progress = trackProgress;
        const auto moveInfo = GetMoveInfo();
        auto unk = CoordsXYZ{ moveInfo->x, moveInfo->y, moveInfo->z } + TrackLocation;

        uint8_t remainingDistanceFlags = 0;
        unk.z += GetRideTypeDescriptor(curRide->type).Heights.VehicleZOffset;
        if (unk.x != _vehicleCurPosition.x)
            remainingDistanceFlags |= (1 << 0);
        if (unk.y != _vehicleCurPosition.y)
            remainingDistanceFlags |= (1 << 1);
        if (unk.z != _vehicleCurPosition.z)
            remainingDistanceFlags |= (1 << 2);

        remaining_distance += SubpositionTranslationDistances[remainingDistanceFlags];
        _vehicleCurPosition.x = unk.x;
        _vehicleCurPosition.y = unk.y;
        _vehicleCurPosition.z = unk.z;

        Orientation = moveInfo->direction;
        bank_rotation = moveInfo->bank_rotation;
        Pitch = moveInfo->Pitch;

        if (remaining_distance < 0)
        {
            acceleration += AccelerationFromPitch[Pitch];
        }
    }
    return true;
}

/**
 *
 *  rct2: 0x006DEF56
 */
int32_t Vehicle::CableLaunchUpdateTrackMotion()
{
    _vehicleF64E2C = 0;
    gCurrentVehicle = this;
    _vehicleMotionTrackFlags = 0;
    _vehicleStationIndex = StationIndex::GetNull();

    velocity += acceleration;
    _vehicleVelocityF64E08 = velocity;
    _vehicleVelocityF64E0C = (velocity / 1024) * 42;

    Vehicle* frontVehicle = this;
    if (velocity < 0)
    {
        frontVehicle = TrainTail();
    }

    _vehicleFrontVehicle = frontVehicle;

    for (Vehicle* vehicle = frontVehicle; vehicle != nullptr;)
    {
        vehicle->acceleration = AccelerationFromPitch[vehicle->Pitch];
        _vehicleUnkF64E10 = 1;
        vehicle->remaining_distance += _vehicleVelocityF64E0C;

        if (vehicle->remaining_distance < 0 || vehicle->remaining_distance >= 13962)
        {
            _vehicleCurPosition = vehicle->GetLocation();
            vehicle->Invalidate();

            while (true)
            {
                if (vehicle->remaining_distance < 0)
                {
                    if (vehicle->CableLaunchUpdateTrackMotionBackwards())
                    {
                        break;
                    }

                    _vehicleMotionTrackFlags |= VEHICLE_UPDATE_MOTION_TRACK_FLAG_5;
                    _vehicleVelocityF64E0C -= vehicle->remaining_distance - 13962;
                    vehicle->remaining_distance = 13962;
                    vehicle->acceleration += AccelerationFromPitch[vehicle->Pitch];
                    _vehicleUnkF64E10++;
                    continue;
                }

                if (vehicle->CableLaunchUpdateTrackMotionForwards())
                {
                    break;
                }

                _vehicleMotionTrackFlags |= VEHICLE_UPDATE_MOTION_TRACK_FLAG_5;
                _vehicleVelocityF64E0C -= vehicle->remaining_distance + 1;
                vehicle->remaining_distance = -1;
                vehicle->acceleration += AccelerationFromPitch[vehicle->Pitch];
                _vehicleUnkF64E10++;
            }
            vehicle->MoveTo(_vehicleCurPosition);
        }
        vehicle->acceleration /= _vehicleUnkF64E10;
        if (_vehicleVelocityF64E08 >= 0)
        {
            vehicle = GetEntity<Vehicle>(vehicle->next_vehicle_on_train);
        }
        else
        {
            if (vehicle == this)
                break;
            vehicle = GetEntity<Vehicle>(vehicle->prev_vehicle_on_ride);
        }
    }

    uint32_t vehicleCount = 0;
    uint16_t massTotal = 0;
    int32_t accelerationTotal = 0;

    for (Vehicle* vehicle = GetEntity<Vehicle>(Id); vehicle != nullptr;
         vehicle = GetEntity<Vehicle>(vehicle->next_vehicle_on_train))
    {
        vehicleCount++;

        massTotal += vehicle->mass;
        accelerationTotal = AddClamp_int32_t(accelerationTotal, vehicle->acceleration);
    }

    int32_t newAcceleration = (accelerationTotal / vehicleCount) >> 9;
    newAcceleration -= velocity >> 12;

    int32_t edx = velocity >> 8;
    edx *= edx;
    if (velocity < 0)
    {
        edx = -edx;
    }
    edx >>= 4;
    newAcceleration -= edx / massTotal;

    acceleration = newAcceleration;
    return _vehicleMotionTrackFlags;
}
