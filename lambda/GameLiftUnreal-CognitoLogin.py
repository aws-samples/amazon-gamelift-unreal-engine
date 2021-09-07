import boto3

# Change to your own pool and client name
USER_POOL_NAME = 'gameliftunreal-userpool'
USER_POOL_CLIENT_NAME = 'gameliftunreal-login-lambda-client'

client = boto3.client('cognito-idp')

def lambda_handler(event, context):
    username = event['username']
    if 'password' in event:
        resp, msg = initiate_auth(username, event['password'])
    if msg != None:
        return {
            'status': 'fail', 
            'msg': msg
        }
    response = {
        'status': 'success',
        'tokens': resp['AuthenticationResult']
#        'id_token': resp['AuthenticationResult']['IdToken'],
#        'access_token': resp['AuthenticationResult']['AccessToken']
    }
    return response

def initiate_auth(username, password):
    try:
        user_pool_id = get_cognito_pool_id(USER_POOL_NAME)
        user_pool_client_id = get_cognito_user_pool_client(user_pool_id, USER_POOL_CLIENT_NAME)
        resp = client.admin_initiate_auth(
            UserPoolId=user_pool_id,
            ClientId=user_pool_client_id,
            AuthFlow='ADMIN_USER_PASSWORD_AUTH',
            AuthParameters={
                'USERNAME': username,
                'PASSWORD': password
            })
    except (client.exceptions.NotAuthorizedException, client.exceptions.UserNotFoundException) as e:
        return None, "The username or password is incorrect"
    except Exception as e:
        return None, "Unknown error"
    return resp, None

def get_cognito_pool_id(pool_name):
    user_pools = client.list_user_pools(MaxResults=20)['UserPools']
    gamelift_user_pool = list(filter(lambda pool: pool['Name'] == pool_name, user_pools))
    if len(gamelift_user_pool) < 1:
        return None
        #raise Exception("Cognito user pool not found")
    return gamelift_user_pool[0]['Id']

def get_cognito_user_pool_client(pool_id, client_name):
    pool_clients = client.list_user_pool_clients(UserPoolId=pool_id)['UserPoolClients']
    login_client_agent = list(filter(lambda pool_client: pool_client['ClientName'] == client_name, pool_clients))
    if len(pool_clients) < 1:
        return None
        #raise Exception("Cognito pool agent not found")
    return login_client_agent[0]['ClientId']
