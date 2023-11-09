void processPages() {
  switch (pager.getCurrentPageId()) {
    case MAIN_PAGE_ID: mainPageProcess(); break;
    case DATETIME_PAGE_ID: datetimePageProcess(); break;
    case DETAILS_PAGE_ID: detailsPageProcess(); break;
    case MENU_PAGE_ID: menuPageProcess(); break;
    case SET_VALUES_PAGE_ID: setValuesPageProcess(); break;
    case PROGRESSBAR_PAGE_ID: progressBarPageProcess(); break;
    case GRAPH_PAGE_ID: graphPageProcess(); break;
    case CUSTOM_VIEW_PAGE_ID: customViewPageProcess(); break;
    case RESET_VALUES_PAGE_ID: resetValuesPageProcess(); break;
  }
}

void mainPageProcess() {
  if (state.editTitleOnPageInit) {
    state.editTitleOnPageInit = false;
    mainPage.findAndSelectEnabledItemById(mainTitleView.getId());
    mainTitleView.beginEditing();
  }

  if (enc.isTurn()) {
    if (mainPage.getSelectedItemId() == mainTitleView.getId() && mainTitleView.isEditing()) {
      if (enc.isRight()) {
        state.editTitleOnPageInit = true;
        pager.navigateTo(DATETIME_PAGE_ID);
      }
    } else {
      if (enc.isRight()) {
        mainPage.nextItem();
      } else {
        mainPage.prevItem();
      }
    }
  }

  if (enc.hasClicks(1)) {
    switch (mainPage.getSelectedItemId()) {
      case MENU_CHARMAP_VIEW_ID:
        pager.navigateTo(MENU_PAGE_ID);
        break;
      case MAIN_TITLE_VIEW_ID:
        !mainTitleView.isEditing() ? mainTitleView.beginEditing() : mainTitleView.endEditing();
        break;
    }
  }
}

void datetimePageProcess() {
  if (state.editTitleOnPageInit) {
    state.editTitleOnPageInit = false;
    datetimePage.findAndSelectEnabledItemById(datetimeTitleView.getId());
    datetimeTitleView.beginEditing();
  }

  if (enc.isTurn()) {
    if (datetimePage.getSelectedItemId() == datetimeTitleView.getId() && datetimeTitleView.isEditing()) {
      state.editTitleOnPageInit = true;
      pager.navigateTo(enc.isRight() ? DETAILS_PAGE_ID : MAIN_PAGE_ID);
    } else {
      if (enc.isRight()) {
        datetimePage.nextItem();
      } else {
        datetimePage.prevItem();
      }
    }
  }

  if (enc.hasClicks(1)) {
    switch (datetimePage.getSelectedItemId()) {
      case DATETIME_TITLE_VIEW_ID:
        !datetimeTitleView.isEditing() ? datetimeTitleView.beginEditing() : datetimeTitleView.endEditing();
        break;
    }
  }
}

void detailsPageProcess() {
  if (state.editTitleOnPageInit) {
    state.editTitleOnPageInit = false;
    detailsPage.findAndSelectEnabledItemById(detailsTitleView.getId());
    detailsTitleView.beginEditing();
  }

  if (enc.isTurn()) {
    if (detailsPage.getSelectedItemId() == detailsTitleView.getId() && detailsTitleView.isEditing()) {
      if (enc.isLeft()) {
        state.editTitleOnPageInit = true;
        pager.navigateTo(DATETIME_PAGE_ID);
      }
    } else {
      if (enc.isRight()) {
        detailsPage.nextItem();
      } else {
        detailsPage.prevItem();
      }
    }
  }

  if (enc.hasClicks(1)) {
    switch (detailsPage.getSelectedItemId()) {
      case DETAILS_TITLE_VIEW_ID:
        !detailsTitleView.isEditing() ? detailsTitleView.beginEditing() : detailsTitleView.endEditing();
        break;
    }
  }
}

void menuPageProcess() {
  if (enc.isTurn()) {
    if (enc.isRight()) {
      menuPage.nextItem();
    } else {
      menuPage.prevItem();
    }
  }

  if (enc.hasClicks(1)) {
    switch (menuPage.getSelectedItemId()) {
      case BACK_CHARMAP_VIEW_ID:
        pager.navigateTo(MAIN_PAGE_ID);
        break;
      case SET_VALUES_MENU_ITEM_VIEW_ID:
        pager.navigateTo(SET_VALUES_PAGE_ID);
        break;
      case PROGRESSBAR_MENU_ITEM_VIEW_ID:
        pager.navigateTo(PROGRESSBAR_PAGE_ID);
        break;
      case GRAPH_MENU_ITEM_VIEW_ID:
        pager.navigateTo(GRAPH_PAGE_ID);
        break;
      case CUSTOM_VIEW_MENU_ITEM_VIEW_ID:
        pager.navigateTo(CUSTOM_VIEW_PAGE_ID);
        break;
      case RESET_VALUES_MENU_ITEM_VIEW_ID:
        pager.navigateTo(RESET_VALUES_PAGE_ID);
        break;
    }
  }
}

void setValuesPageProcess() {
  if (enc.isTurn()) {
    if (setValuesPage.getSelectedItemId() == textValueLabelView.getId() && textValueLabelView.isEditing()) {
      enc.isRight() ? state.values.textValue++ : state.values.textValue--;
    } else if (setValuesPage.getSelectedItemId() == charValueView.getId() && charValueView.isEditing()) {
      enc.isRight() ? state.values.charValue++ : state.values.charValue--;
    } else {
      if (enc.isRight()) {
        setValuesPage.nextItem();
      } else {
        setValuesPage.prevItem();
      }
    }
  }

  if (enc.hasClicks(1)) {
    switch (setValuesPage.getSelectedItemId()) {
      case BACK_CHARMAP_VIEW_ID:
        pager.navigateTo(MENU_PAGE_ID);
        break;
      case TEXT_VALUE_LABEL_VIEW_ID:
        !textValueLabelView.isEditing() ? textValueLabelView.beginEditing() : textValueLabelView.endEditing();
        break;
      case CHAR_VALUE_VIEW_ID:
        !charValueView.isEditing() ? charValueView.beginEditing() : charValueView.endEditing();
        break;
      case SWITCH_VIEW_ID:
        state.values.switchValue = !state.values.switchValue;
        break;
      case CHECKBOX_VIEW_ID:
        state.values.checkBoxValue = !state.values.checkBoxValue;
        break;
    }
  }
}

void progressBarPageProcess() {
  if (enc.isTurn()) {
    if (progressBarPage.getSelectedItemId() == progressBarView.getId() && progressBarView.isEditing()) {
      enc.isRight() ? state.values.progressBarValue++ : state.values.progressBarValue--;
    } else {
      if (enc.isRight()) {
        progressBarPage.nextItem();
      } else {
        progressBarPage.prevItem();
      }
    }
  }

  if (enc.hasClicks(1)) {
    switch (progressBarPage.getSelectedItemId()) {
      case BACK_CHARMAP_VIEW_ID:
        pager.navigateTo(MENU_PAGE_ID);
        break;
      case PROGRESSBAR_VIEW_ID:
        !progressBarView.isEditing() ? progressBarView.beginEditing() : progressBarView.endEditing();
        break;
    }
  }
}

void graphPageProcess() {
  if (enc.isTurn()) {
    if (graphPage.getSelectedItemId() == graphRefreshView.getId() && graphRefreshView.isEditing()) {
      byte length = sizeof(state.values.graphValues);
      enc.isRight() ? shiftArrayRight(state.values.graphValues, length) : shiftArrayLeft(state.values.graphValues, length);
      graphView.notifyDataChanged();
    } else {
      if (enc.isRight()) {
        graphPage.nextItem();
      } else {
        graphPage.prevItem();
      }
    }
  }

  if (enc.hasClicks(1)) {
    switch (graphPage.getSelectedItemId()) {
      case BACK_CHARMAP_VIEW_ID:
        pager.navigateTo(MENU_PAGE_ID);
        break;
      case GRAPH_REFRESH_VIEW_ID:
        !graphRefreshView.isEditing() ? graphRefreshView.beginEditing() : graphRefreshView.endEditing();
        break;
    }
  }
}

void customViewPageProcess() {
  if (enc.isTurn()) {
    if (enc.isRight()) {
      customViewPage.nextItem();
    } else {
      customViewPage.prevItem();
    }
  }

  if (enc.hasClicks(1)) {
    switch (customViewPage.getSelectedItemId()) {
      case BACK_CHARMAP_VIEW_ID:
        pager.navigateTo(MENU_PAGE_ID);
        break;
    }
  }
}

void resetValuesPageProcess() {
  if (enc.isTurn()) {
    if (enc.isRight()) {
      resetValuesPage.nextItem();
    } else {
      resetValuesPage.prevItem();
    }
  }

  if (enc.hasClicks(1)) {
    switch (resetValuesPage.getSelectedItemId()) {
      case BACK_CHARMAP_VIEW_ID:
        pager.navigateTo(MENU_PAGE_ID);
        break;
      case RESET_VALUES_OK_VIEW_ID:
        state.values = Values();
        pager.navigateTo(MENU_PAGE_ID);
        break;
    }
  }
}
