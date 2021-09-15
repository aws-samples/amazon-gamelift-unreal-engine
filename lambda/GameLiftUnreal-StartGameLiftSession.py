# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: MIT-0

import json
import boto3
import uuid
import datetime
import time

GAMELIFT_FLEET_ID = ""

game_lift = boto3.client("gamelift")

def lambda_handler(event, context):
    response = {}
    # Find an game session with availability then create a new player session on it
    game_session = find_available_game_session()
    if (game_session is not None):
        player_session = game_lift.create_player_session(GameSessionId =  game_session['GameSessionId'], PlayerId = str(uuid.uuid4()))
        response = player_session
    else:
        return {
            'statusCode': 500,
            'body': json.dumps('Unable to find game session, check GameLift API status')
        }
    return json.dumps(response, default = myconverter).encode('UTF-8')

# Find existing game session with available player sessions, otherwise create a new game session on the fleet
def find_available_game_session():
    game_sessions = game_lift.search_game_sessions(FleetId = GAMELIFT_FLEET_ID, FilterExpression = "hasAvailablePlayerSessions=true")
    game_session = {}
    if (len(game_sessions['GameSessions']) == 0):
        # Create a new game session
        game_session = game_lift.create_game_session(FleetId = GAMELIFT_FLEET_ID, MaximumPlayerSessionCount = 16)['GameSession']
        # Wait for game session status to leave ACTIVATING state
        game_session_status = "ACTIVATING"
        while (game_session_status == "ACTIVATING"):
            try:
                session_details = game_lift.describe_game_session_details(GameSessionId = game_session['GameSessionId'])
                game_session_status = session_details['GameSessionDetails'][0]['GameSession']['Status']
            except:
                break
            # Sleep 10 ms between describe calls
            time.sleep(0.01)
    else:
        # Use existing game session
        game_session = game_sessions['GameSessions'][0]
    return game_session
    
def myconverter(o):
    if isinstance(o, datetime.datetime):
        return o.__str__()