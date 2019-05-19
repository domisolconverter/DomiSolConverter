from django.shortcuts import render, reverse, HttpResponseRedirect, redirect
from .forms import UploadFileForm
from django.http import HttpResponseRedirect

# Create your views here.

def main(request):

    if request.method == 'POST':
        form = UploadFileForm(request.POST, request.FILES)
        if form.is_valid():
            uploadFile = request.FILES['file']
            # pdf 파일이 아니라면 작업을 멈추고 리턴
            if uploadFile.name.find('pdf') < 0 :
                message = "파일 형식이 잘못 되었습니다. pdf 파일을 업로드하세요."
                return render(request, 'main.html', { 'form' : form, "message": message })
            # pdf 파일이라면 저장
            else:
                form.save()
                # (수정) loading 함수로 이동 -> loading 화면 띄우고 악보 인식 시작
                redirect_to = reverse('main')
                return HttpResponseRedirect(redirect_to)


        else:
            form = UploadFileForm()
            ####### (수정) 이 부분이 form.save() 밑에 들어가야 합니다! ###########
            print('수정해라 예솔아~~~~');
            redirect_to = reverse('uploading')
            return HttpResponseRedirect(redirect_to)

    return render(request, 'main.html', { })


# 소켓 연결해서 로딩화면 띄우면서 서버에 있는 프로그램 돌리고 서버로부터 데이터 받아오는 것이 필요하당
def process_upload(request):

    return render(request, 'uploading.html', {})
