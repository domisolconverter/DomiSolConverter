from django.shortcuts import render, reverse, HttpResponseRedirect, redirect
from django.http import HttpResponseRedirect
from main.models import *


# Create your views here.
def edit(request):

    # 업로드 ID로 결과 이미지들을 찾은 다음 리스트로 반환

    return render(request, 'edit.html', {})
