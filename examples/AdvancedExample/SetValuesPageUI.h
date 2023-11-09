#define SET_VALUES_TITLE_VIEW_ID 1

#define TEXT_VALUE_LABEL_VIEW_ID 2
#define TEXT_VALUE_VIEW_ID 3

#define CHAR_VALUE_LABEL_VIEW_ID 4
#define CHAR_VALUE_VIEW_ID 5

#define SWITCH_LABEL_VIEW_ID 6
#define SWITCH_VIEW_ID 7

#define CHECKBOX_LABEL_VIEW_ID 8
#define CHECKBOX_VIEW_ID 9

String getTextValue() {
  return (String)state.values.textValue;
}

char getCharValue() {
  return state.values.charValue;
}

bool getSwitchValue() {
  return state.values.switchValue;
}

bool getCheckBoxValue() {
  return state.values.checkBoxValue;
}

LcdFlashTextView setValuesTitleView(&lcdI2C, SET_VALUES_TITLE_VIEW_ID, LCD_CENTER_HORIZONTALLY, LCD_AUTO_VERTICALLY, false);

LcdFlashTextView textValueLabelView(&lcdI2C, TEXT_VALUE_LABEL_VIEW_ID, 1, LCD_AUTO_VERTICALLY);
LcdTextValueView textValueView(&lcdI2C, TEXT_VALUE_VIEW_ID, 6, LCD_AUTO_VERTICALLY, 3, getTextValue, false);

LcdFlashTextView charValueLabelView(&lcdI2C, CHAR_VALUE_LABEL_VIEW_ID, 1, LCD_AUTO_VERTICALLY, false);
LcdCharValueView charValueView(&lcdI2C, CHAR_VALUE_VIEW_ID, 6, LCD_AUTO_VERTICALLY, getCharValue);

LcdSwitchView switchView(&lcdI2C, SWITCH_VIEW_ID, 1, LCD_AUTO_VERTICALLY, getSwitchValue);
LcdFlashTextView switchLabelView(&lcdI2C, SWITCH_LABEL_VIEW_ID, 4, LCD_AUTO_VERTICALLY, false);

LcdCheckBoxView checkBoxView(&lcdI2C, CHECKBOX_VIEW_ID, 1, LCD_AUTO_VERTICALLY, getCheckBoxValue);
LcdFlashTextView checkBoxLabelView(&lcdI2C, CHECKBOX_LABEL_VIEW_ID, 3, LCD_AUTO_VERTICALLY, false);

LcdPageRow* setValuesPageRows[] = {
  new LcdPageRow(new LiquidCrystalView*[2]{ &backCharmapView, &setValuesTitleView }, 2),
  new LcdPageRow(new LiquidCrystalView*[2]{ &textValueLabelView, &textValueView }, 2),
  new LcdPageRow(new LiquidCrystalView*[2]{ &charValueLabelView, &charValueView }, 2),
  new LcdPageRow(new LiquidCrystalView*[2]{ &switchView, &switchLabelView }, 2),
  new LcdPageRow(new LiquidCrystalView*[2]{ &checkBoxView, &checkBoxLabelView }, 2),
};
LcdPageLayout setValuesPageLayout = LcdPageLayout(setValuesPageRows, 5);
LiquidCrystalPage setValuesPage = LiquidCrystalPage(&lcdI2C, SET_VALUES_PAGE_ID, &setValuesPageLayout);
