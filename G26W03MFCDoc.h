// G26W03MFCDoc.h: CG26W03MFCDoc 클래스의 인터페이스
//

#pragma once
#include <vector>

// 개별 원의 좌표와 반지름 정보를 담는 구조체
struct CircleData {
	int x;
	int y;
	int radius;
};

class CG26W03MFCDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CG26W03MFCDoc() noexcept;
	DECLARE_DYNCREATE(CG26W03MFCDoc)

	// 특성입니다.
public:

	// 작업입니다.
public:

	// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

	// 구현입니다.
public:
	virtual ~CG26W03MFCDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// 누적된 원 데이터를 저장하는 컨테이너
	std::vector<CircleData> m_circles;

public:
	// View에서 그릴 때 참조할 Getter
	const std::vector<CircleData>& GetCircles() const { return m_circles; }

	// 원 데이터 추가 (좌클릭 시 호출)
	void AddCircle(int x, int y, int radius)
	{
		CircleData circle = { x, y, radius };
		m_circles.push_back(circle);
		SetModifiedFlag();
	}

	// 가장 최근에 생성된 원 제거 (우클릭 Undo 시 호출)
	void RemoveLastCircle()
	{
		if (!m_circles.empty())
		{
			m_circles.pop_back();
			SetModifiedFlag();
		}
	}

	// 전체 원 데이터 초기화
	void ClearCircles()
	{
		m_circles.clear();
		SetModifiedFlag();
	}

	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};