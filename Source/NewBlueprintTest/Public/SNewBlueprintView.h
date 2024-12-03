#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"


class SNewBlueprintView : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SNewBlueprintView)
        {}

    SLATE_END_ARGS()

public:
    SNewBlueprintView() {}
    ~SNewBlueprintView();

    void Construct(const FArguments& InArgs);

    // SWidget interface
    virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
    // End of SWidget interface


    // ��ť���½���ͼ�ࡱ�����Ӧ
    FReply NewBlueprintClicked();

    // ��ť���洢���ԡ������Ӧ��ʵ����һ��Blueprint���͵�ʵ��TempObject�������༭��PropertyInput�е��ַ��������л���TempObject�������С�
    FReply SaveToProperty();

    // ��ť���޸����ԡ������Ӧ����ԭ�����Ƴ���������������ԣ���������ͼ��
    FReply ModifyProperty();

    // ��ť����ȡ���ԡ������Ӧ�����Ե���������TempObject
    FReply LoadProperty();

private:

    TSharedPtr<UBlueprint> Blueprint; // ��̬��������ͼ��

    //TSharedPtr<UBlueprintGeneratedClass> GeneratedClass;

    UObject* TempObject; // ��̬������Blueprint���͵Ķ������ڴ�ȡProperty

    TSharedPtr<SEditableTextBox> PropertyInput; // ��������

    TSharedPtr<SVerticalBox> PropertyOutputBox; // ���������
};
