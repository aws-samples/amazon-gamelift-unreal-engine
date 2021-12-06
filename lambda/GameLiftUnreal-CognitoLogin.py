# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: MIT-0

import boto3
import os

# TODO Set to your created app client id. For testing purposes, it is possible to use a hardcoded string here, but please consider using an environment variable.
# i.e. USER_POOL_APP_CLIENT_ID = os.environ['USER_POOL_APP_CLIENT_ID']
# (See https://docs.aws.amazon.com/lambda/latest/dg/configuration-envvars.html for info on programmatically defining environment variables in Lambda.)
USER_POOL_APP_CLIENT_ID = ''

client = boto3.client('cognito-idp')

def lambda_handler(event, context):
    username = event['username']
    msg = "Password required"
    resp = {}
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
    }
    return response

def initiate_auth(username, password):
    try:
        resp = client.initiate_auth(
            ClientId=USER_POOL_APP_CLIENT_ID,
            AuthFlow='USER_PASSWORD_AUTH',
            AuthParameters={
                'USERNAME': username,
                'PASSWORD': password
            })
    except (client.exceptions.NotAuthorizedException, client.exceptions.UserNotFoundException) as e:
        return None, "The username or password is incorrect"
    except Exception as e:
        return None, "Unknown error"
    return resp, None
