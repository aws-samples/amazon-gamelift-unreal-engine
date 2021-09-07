import json
import boto3
import uuid
import datetime

game_lift = boto3.client("gamelift")

def lambda_handler(event, context):
    response = {}
    game_sessions = []
    game_session = {}
    # Getting Fleet ID 
    fleets = game_lift.list_fleets()
    fleet_id = fleets['FleetIds'][0]
    # Searches for a Game Session on the Fleet
    game_sessions = game_lift.search_game_sessions(FleetId = fleet_id, FilterExpression = "hasAvailablePlayerSessions=true")
    if(len(game_sessions['GameSessions']) == 0):
        game_session = game_lift.create_game_session(FleetId = fleet_id, MaximumPlayerSessionCount = 80)
    else:
        game_session = game_sessions['GameSessions'][0]
    if(game_session is not None):
        player_session = game_lift.create_player_session(GameSessionId =  game_session['GameSessionId'], PlayerId = str(uuid.uuid4()))
        response = player_session
    else:
        return {
            'statusCode': 500,
            'body': json.dumps('Unable to find game session, check GameLift API status')
        }
    return json.dumps(response, default = myconverter).encode('UTF-8')
def myconverter(o):
    if isinstance(o, datetime.datetime):
        return o.__str__()