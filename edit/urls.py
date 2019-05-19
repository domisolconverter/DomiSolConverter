from django.urls import path
from django.conf.urls import url
from . import views

urlpatterns = [
    path('edit/', views.edit, name='edit'),
    url(r'^edit/(?P<Upload_id>\d+)/$', views.edit, name='edit'),

]
