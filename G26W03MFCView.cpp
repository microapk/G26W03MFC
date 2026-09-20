// G26W03MFCView.cpp: CG26W03MFCView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "G26W03MFC.h"
#endif

#include "G26W03MFCDoc.h"
#include "G26W03MFCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CG26W03MFCView

IMPLEMENT_DYNCREATE(CG26W03MFCView, CView)

BEGIN_MESSAGE_MAP(CG26W03MFCView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN() // 우클릭 메시지 맵 등록
	ON_WM_KEYDOWN()     // 키보드 입력 메시지 맵 등록
END_MESSAGE_MAP()

// CG26W03MFCView 생성/소멸

CG26W03MFCView::CG26W03MFCView() noexcept
{
	m_currentRadius = 30; // 기본 반지름 초기화
}

CG26W03MFCView::~CG26W03MFCView()
{
}

BOOL CG26W03MFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// CG26W03MFCView 그리기

void CG26W03MFCView::OnDraw(CDC* pDC)
{
	CG26W03MFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// GDI 그래픽 객체 설정 (노란색 브러시, 파란색 테두리 펜)
	CBrush yellowBrush(RGB(255, 255, 0));
	CPen bluePen(PS_SOLID, 2, RGB(0, 0, 255));

	CBrush* pOldBrush = pDC->SelectObject(&yellowBrush);
	CPen* pOldPen = pDC->SelectObject(&bluePen);

	// Document에 저장된 모든 원을 순회하며 일괄 렌더링
	const auto& circles = pDoc->GetCircles();
	for (const auto& c : circles)
	{
		pDC->Ellipse(c.x - c.radius, c.y - c.radius, c.x + c.radius, c.y + c.radius);
	}

	// 기존 GDI 객체로 복원
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}


// CG26W03MFCView 인쇄

BOOL CG26W03MFCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CG26W03MFCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CG26W03MFCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}


// CG26W03MFCView 진단

#ifdef _DEBUG
void CG26W03MFCView::AssertValid() const
{
	CView::AssertValid();
}

void CG26W03MFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CG26W03MFCDoc* CG26W03MFCView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CG26W03MFCDoc)));
	return (CG26W03MFCDoc*)m_pDocument;
}
#endif //_DEBUG


// CG26W03MFCView 메시지 처리기

// 1. 마우스 좌클릭: 현재 반지름 크기로 Document에 원 데이터 추가
void CG26W03MFCView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CG26W03MFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	pDoc->AddCircle(point.x, point.y, m_currentRadius);
	Invalidate(); // 전체 영역 무효화 및 OnDraw 호출 요청

	CView::OnLButtonDown(nFlags, point);
}

// 2. 마우스 우클릭: 가장 최근에 생성된 원 데이터 삭제 (Undo)
void CG26W03MFCView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CG26W03MFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	pDoc->RemoveLastCircle();
	Invalidate(); // 갱신된 Document 데이터를 기반으로 재출력 요청

	CView::OnRButtonDown(nFlags, point);
}

// 3. 키보드 입력: 상/하 방향키로 생성할 원의 크기 조절
void CG26W03MFCView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_UP)
	{
		m_currentRadius += 5;
		if (m_currentRadius > 100) m_currentRadius = 100; // 최대 반지름 제한
	}
	else if (nChar == VK_DOWN)
	{
		m_currentRadius -= 5;
		if (m_currentRadius < 10) m_currentRadius = 10;   // 최소 반지름 제한
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}