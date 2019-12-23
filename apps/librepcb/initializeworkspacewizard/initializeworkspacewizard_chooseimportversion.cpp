/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 LibrePCB Developers, see AUTHORS.md for contributors.
 * https://librepcb.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*******************************************************************************
 *  Includes
 ******************************************************************************/
#include "initializeworkspacewizard_chooseimportversion.h"

#include "librepcb/workspace/workspace.h"
#include "ui_initializeworkspacewizard_chooseimportversion.h"

/*******************************************************************************
 *  Namespace
 ******************************************************************************/
namespace librepcb {
namespace application {

using namespace workspace;

/*******************************************************************************
 *  Constructors / Destructor
 ******************************************************************************/

InitializeWorkspaceWizard_ChooseImportVersion::
    InitializeWorkspaceWizard_ChooseImportVersion(
        InitializeWorkspaceWizardContext& context, QWidget* parent) noexcept
  : QWizardPage(parent),
    mContext(context),
    mUi(new Ui::InitializeWorkspaceWizard_ChooseImportVersion) {
  mUi->setupUi(this);

  mUi->cbxVersions->addItem(tr("Do not import any data"));
  foreach (const Version& version, Workspace::getFileFormatVersionsOfWorkspace(
                                       mContext.getWorkspacePath())) {
    mUi->cbxVersions->addItem("LibrePCB " % version.toStr() % ".x",
                              version.toStr());
  }

  connect(
      mUi->cbxVersions,
      static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
      this,
      &InitializeWorkspaceWizard_ChooseImportVersion::
          cbxVersionCurrentIndexChanged);

  mUi->cbxVersions->setCurrentIndex(mUi->cbxVersions->count() - 1);
}

InitializeWorkspaceWizard_ChooseImportVersion::
    ~InitializeWorkspaceWizard_ChooseImportVersion() noexcept {
}

/*******************************************************************************
 *  Public Methods
 ******************************************************************************/

void InitializeWorkspaceWizard_ChooseImportVersion::initializePage() {
  updateButtons();
}

/*******************************************************************************
 *  Private Methods
 ******************************************************************************/

void InitializeWorkspaceWizard_ChooseImportVersion::
    cbxVersionCurrentIndexChanged(int index) noexcept {
  mContext.setVersionToImport(
      Version::tryFromString(mUi->cbxVersions->itemData(index).toString()));
  updateButtons();
}

void InitializeWorkspaceWizard_ChooseImportVersion::updateButtons() noexcept {
  bool lastPage = mContext.getVersionToImport().has_value();
  setFinalPage(lastPage);
  if (QAbstractButton* btn =
          wizard() ? wizard()->button(QWizard::NextButton) : nullptr) {
    btn->setEnabled(!lastPage);
  }
}

/*******************************************************************************
 *  End of File
 ******************************************************************************/

}  // namespace application
}  // namespace librepcb
