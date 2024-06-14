import http.client
import ssl
import json
import requests

ssl._create_default_https_context = ssl._create_unverified_context

def get_key():
    conn = http.client.HTTPSConnection("dev-0kbye71s8zz2tmv6.us.auth0.com")
    payload = "{\"client_id\":\"pSN03Jc5fvvTuAcNOdn8XRrEq5tIHzvW\",\"client_secret\":\"4lS1bMWCaDZnNPUfQOCpd-cOfjuIXZtfrILFdzjtFSfi_XQPIW75jxR_cXK-LDQH\",\"audience\":\"https://dev-0kbye71s8zz2tmv6.us.auth0.com/api/v2/\",\"grant_type\":\"client_credentials\"}"
    headers = { 'content-type': "application/json" }
    conn.request("POST", "/oauth/token", payload, headers)
    res = conn.getresponse()
    data = res.read()
    return json.loads(data.decode("utf-8"))["access_token"]

def get_all_users():
    url = "https://dev-0kbye71s8zz2tmv6.us.auth0.com/api/v2/users"
    payload = {}
    headers = {
    'Accept': 'application/json',
    'Authorization': f'Bearer {get_key()}'
    }
    response = requests.request("GET", url, headers=headers, data=payload)
    return json.loads(response.text)

def get_user_by_id(user_id):
    url = f"https://dev-0kbye71s8zz2tmv6.us.auth0.com/api/v2/users/{user_id}"
    payload = {}
    headers = {
    'Accept': 'application/json',
    'Authorization': f'Bearer {get_key()}'
    }  
    response = requests.request("GET", url, headers=headers, data=payload)
    return json.loads(response.text)

def get_user_roles_by_id(user_id):
    url = f"https://dev-0kbye71s8zz2tmv6.us.auth0.com/api/v2/users/{user_id}/roles"
    payload = {}
    headers = {
    'Accept': 'application/json',
    'Authorization': f'Bearer {get_key()}'
    }
    response = requests.request("GET", url, headers=headers, data=payload)
    return json.loads(response.text)

def delete_user_by_id(user_id):
    url = f"https://dev-0kbye71s8zz2tmv6.us.auth0.com/api/v2/users/{user_id}"
    payload = {}
    headers = {
    'Authorization': f'Bearer {get_key()}'
    }
    response = requests.request("DELETE", url, headers=headers, data=payload)
    return response.status_code