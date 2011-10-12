# 
# These are just a couple Django-specific utility methods 
# abstracted to perform common tasks in as generic way as possible.
#

from datetime import datetime, timedelta

from django.conf import settings
from django.shortcuts import render_to_response as django_render_to_response
from django.core.paginator import Paginator, EmptyPage, PageNotAnInteger
from django.template import RequestContext
from django.core.cache import cache
from django.contrib.auth.models import User


def render_to_response(template, payload):
   return django_render_to_response(template, payload, 
     context_instance=RequestContext(payload['request']))

def generate_paginator_and_page(query, page_num, items_per_page=10, orphans=1):
   paginator = Paginator(query, items_per_page, orphans)
   
   try:
      objects = paginator.page(page_num)
   except PageNotAnInteger:
      objects = paginator.page(1)
   except EmptyPage:
      objects = paginator.page(paginator.num_pages)
   
   return (paginator, objects)

def get_or_create_user(email_address):
   """Returns a user based on their email address, or creates one and returns it."""
   user, user_created = User.objects.get_or_create(
      email=email_address, 
      defaults={'is_active': False, 'username': email_address }
   )
   if user_created:
      user.set_password('new_user_%d' % user.id) # temporary password
      user.save()
   return user

def set_cookie(response, key, value, days_expire = 7):
   """
   Sets a cookie on the given HTTPResponse. The cookie will 
   be sent to the client attached to the response.
   """
   if days_expire is None: days_expire = 365
   max_age = days_expire * 24 * 60 * 60 
   expires = datetime.strftime(
      datetime.utcnow() + timedelta(seconds=max_age), "%a, %d-%b-%Y %H:%M:%S GMT")
   response.set_cookie(
      key, 
      value, 
      max_age=max_age, 
      expires=expires, 
      domain=settings.SESSION_COOKIE_DOMAIN, 
      secure=settings.SESSION_COOKIE_SECURE or None
   )
   return response

