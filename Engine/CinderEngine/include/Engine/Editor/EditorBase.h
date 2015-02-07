#ifndef EDITORBASE_H_
#define EDITORBASE_H_

class EditorBase
{
public:

	EditorBase( ) { }
	virtual ~EditorBase( ) { };

	virtual void Initialize( ) { }
	virtual void Shutdown( ) { }
	virtual void SetVisible( bool state ) = 0;
	virtual void Refresh( ) = 0;
	virtual void Reset( ) { }
};

#endif