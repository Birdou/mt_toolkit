
#if defined(_WIN32)

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <clocale>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <unordered_map>
#include <thread>
#include <cmath>

#include "regkey.hpp"
#include "sregkey.hpp"

#include "mt_application.hpp"
#include "mt_window.hpp"
#include "mt_label.hpp"
#include "mt_textinput.hpp"
#include "mt_button.hpp"
#include "mt_containers.hpp"

#define PSSWD "S3ridu,25es"
#include "mt_filechooser.hpp"
int main(int argc, char *argv[])
{
	// REQUIRES_ELEVATION;

	std::vector<std::string> args;
	for (int i = 0; i < argc; ++i)
	{
		args.emplace_back(argv[i]);
	}

	std::setlocale(LC_ALL, "Portuguese_Brasil.1252");

	Mt_application app("Block", 800, 510 /*, HIDDEN*/);

	auto &window01 = app.window->createChild("Autenticação", "window01", 200, 200, SKIP_TASKBAR | BORDERLESS | ALWAYS_ON_TOP);
	window01.setDraggable(true);
	auto &password = Mt_textbox::create(window01, window01.width() / 2, window01.height() / 2, 160, 20);
	password.geometry->setAnchor(middle_center);
	password.password = true;
	auto &submitpasswd = Mt_button::create(window01, window01.width() / 2, window01.height() / 2 + 20, 50, 20);
	password.enter = [&submitpasswd]()
	{
		submitpasswd.function();
	};
	auto &passwordLabel = Mt_label::create(window01, password.geometry->destR.x, password.geometry->destR.y - 5);
	passwordLabel.geometry->setAnchor(bottom_left);
	passwordLabel.text = "Senha:";
	auto &feedback = Mt_label::create(window01, window01.width() / 2, 50);
	feedback.geometry->setAnchor(bottom_center);
	feedback.font->color.hex(Red);
	submitpasswd.label->text = "Entrar";
	submitpasswd.geometry->setAnchor(top_center);
	submitpasswd.function = [&]()
	{
		if (password.str() == PSSWD)
		{
			window01.destroy();
			app.window->show();
		}
		else
		{
			feedback.text = "Senha incorreta";
		}
	};
	auto &closeBtn = Mt_button::create(window01, window01.width(), 0, 20, 20);
	closeBtn.label->text = "X";
	closeBtn.setScheme({
		{
			// BACKGROUND
			{255, 255, 255, 255}, // NORMAL
			{232, 17, 35, 255},	  // HOVER
			{241, 112, 122, 255}, // CLICKED
			{0, 0, 0, 0},		  // FOCUSED
			{204, 204, 204, 255}, // DISABLED
		},
		{
			// BORDER
			{0, 0, 0, 64},
			{0, 0, 0, 64},
			{0, 0, 0, 64},
			{0, 0, 0, 0},
			{191, 191, 191, 255},
		},
		{
			// FONT
			{5, 7, 8, 255},
			{255, 255, 255, 255},
			{255, 255, 255, 255},
			{0, 0, 0, 0},
			{131, 131, 131, 255},
		},
	});
	closeBtn.geometry->setAnchor(top_right);
	closeBtn.function = [&]()
	{
		app.stop();
	};

	// system(WMIC computersystem where caption="CurrentPCName" rename "NewPCName")
	// WMIC computersystem where name="%computername%" call joindomainorworkgroup name="Workgroup_Name"

	auto &scroll = Mt_scrollarea::create(*app.window, 5, 5, 540, 500, 540, 520);
	auto &flexbox = Mt_flex::create(*app.window, 10, 10, 500, 200);
	scroll.add(flexbox);

	auto createEntry = [&app, &flexbox](const std::string &text, regkey<DWORD, REG_DWORD> *_key)
	{
		auto &label = Mt_label::create(*app.window, 0, 0);
		auto &button = Mt_button::create(*app.window, 0, 0, 100, 20);
		auto &row = flexbox.createRow();
		row.addWidget(label, 4);
		row[0].alignment = top_left;
		row.addWidget(button);

		label.text = text;

		button.foreignData = _key;
		button.function = [&]()
		{
			auto key = static_cast<regkey<DWORD, REG_DWORD> *>(button.foreignData);
			if (key->queryValue() != key->getEnabledValue() || key->getLastError() != 0)
			{
				button.label->text = "Desboquear";
				row.currentBackgroundColor.hex(LightSalmon);
				key->enable();
			}
			else
			{
				button.label->text = "Bloquear";
				row.currentBackgroundColor.hex(PaleGreen);
				key->disable();
			}
		};

		if (_key->queryValue() == _key->getEnabledValue() && _key->getLastError() == 0)
		{
			button.label->text = "Desbloquear";
			row.currentBackgroundColor.hex(LightSalmon);
		}
		else
		{
			button.label->text = "Bloquear";
			row.currentBackgroundColor.hex(PaleGreen);
		}
	};

	auto HK = HKEY_CURRENT_USER;

	std::unique_ptr<regkey<DWORD, REG_DWORD>> EX_REG01(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoControlPanel"));
	createEntry("Explorer: Painel de controle", EX_REG01.get());
	std::unique_ptr<regkey<DWORD, REG_DWORD>> EX_REG02(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoRun"));
	createEntry("Explorer: Menu executar (Win + R)", EX_REG02.get());
	std::unique_ptr<regkey<DWORD, REG_DWORD>> EX_REG03(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoChangeStartMenu"));
	createEntry("Explorer: Alterações no menu iniciar", EX_REG03.get());
	std::unique_ptr<regkey<DWORD, REG_DWORD>> EX_REG04(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "TaskbarLockAll"));
	createEntry("Explorer: Configurações da barra de tarefas", EX_REG04.get());
	std::unique_ptr<regkey<DWORD, REG_DWORD>> EX_REG05(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoTrayContextMenu"));
	createEntry("Explorer: Botão direito na barra de tarefas", EX_REG05.get());
	std::unique_ptr<regkey<DWORD, REG_DWORD>> EX_REG06(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoSaveSettings"));
	createEntry("Explorer: Não salvar configurações ao sair", EX_REG06.get());
	std::unique_ptr<regkey<DWORD, REG_DWORD>> EX_REG07(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", "NoRecentDocsHistory"));
	createEntry("Explorer: Não manter histórico de arquivos recentes", EX_REG07.get());

	std::unique_ptr<regkey<DWORD, REG_DWORD>> REG05(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\ActiveDesktop", "NoChangingWallPaper"));
	createEntry("Papel de parede", REG05.get());
	std::unique_ptr<regkey<DWORD, REG_DWORD>> REG06(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Policies\\Microsoft\\Windows\\Explorer", "ShowSleepOption", 0, 1));
	createEntry("Opção 'suspender'", REG06.get());
	std::unique_ptr<regkey<DWORD, REG_DWORD>> REG07(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Policies\\Microsoft\\Windows\\Explorer", "DisableContextMenusInStart"));
	createEntry("Botão direito no menu iniciar", REG07.get());
	std::unique_ptr<regkey<DWORD, REG_DWORD>> REG08(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Policies\\Microsoft\\Windows\\WindowsUpdate\\AU", "NoAutoUpdate"));
	createEntry("Atualizações automáticas do Windows Update", REG08.get());
	std::unique_ptr<regkey<DWORD, REG_DWORD>> REG09(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Policies\\Microsoft\\Windows\\Installer", "DisableMSI", 0, 2));
	createEntry("Windows Installer", REG09.get());
	std::unique_ptr<regkey<DWORD, REG_DWORD>> REG10(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", "DisableTaskMgr"));
	createEntry("Acesso ao gerenciador de tarefas", REG10.get());

	std::unique_ptr<regkey<DWORD, REG_DWORD>> WD_REG01(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Policies\\Microsoft\\Windows Defender Security Center\\Account protection", "UILockdown"));
	createEntry("Windows Defender: Proteção de contas", WD_REG01.get());
	std::unique_ptr<regkey<DWORD, REG_DWORD>> WD_REG02(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Policies\\Microsoft\\Windows Defender Security Center\\App and Browser protection", "UILockdown"));
	createEntry("Windows Defender: Controle de aplicativos e do navegador", WD_REG02.get());
	std::unique_ptr<regkey<DWORD, REG_DWORD>> WD_REG03(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Policies\\Microsoft\\Windows Defender Security Center\\Device performance and health", "UILockdown"));
	createEntry("Windows Defender: Desempenho e integridade do dispositivo", WD_REG03.get());
	std::unique_ptr<regkey<DWORD, REG_DWORD>> WD_REG04(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Policies\\Microsoft\\Windows Defender Security Center\\Device security", "UILockdown"));
	createEntry("Windows Defender: Segurança do dispositivo", WD_REG04.get());
	std::unique_ptr<regkey<DWORD, REG_DWORD>> WD_REG05(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Policies\\Microsoft\\Windows Defender Security Center\\Family options", "UILockdown"));
	createEntry("Windows Defender: Opções da família", WD_REG05.get());
	std::unique_ptr<regkey<DWORD, REG_DWORD>> WD_REG06(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Policies\\Microsoft\\Windows Defender Security Center\\Firewall and network protection", "UILockdown"));
	createEntry("Windows Defender: Firewall e proteção de rede", WD_REG06.get());
	std::unique_ptr<regkey<DWORD, REG_DWORD>> WD_REG07(new regkey<DWORD, REG_DWORD>(HK, "SOFTWARE\\Policies\\Microsoft\\Windows Defender Security Center\\Virus and threat protection", "UILockdown"));
	createEntry("Windows Defender: Proteção contra vírus e ameaças", WD_REG07.get());

	auto &restartExplorer = Mt_button::create(*app.window, 570, 10, 200, 30);
	restartExplorer.label->text = "Reiniciar Windows Explorer";
	restartExplorer.function = []()
	{
		system("taskkill /f /im explorer.exe");
		system("start C:\\Windows\\explorer.exe");
	};

	auto &flex = Mt_flex::create(*app.window, 570, 150, 200, 200);
	auto &button01 = Mt_button::create(*app.window, 0, 0, 20, 20);
	auto &label01 = Mt_label::create(*app.window, 0, 0);
	label01.font->color.hex(Yellow);
	label01.text = "Label 01";
	auto &button02 = Mt_button::create(*app.window, 0, 0, 20, 50);
	auto &row = flex.createRow();
	row.currentBackgroundColor.hex(Red).a = 122;
	row.addWidget(button01, 2);
	row[0].alignment = middle_center;
	row.addWidget(label01);
	row.addWidget(button02);
	auto &row2 = flex.createRow();
	row2.currentBackgroundColor.hex(Lime).a = 122;
	auto &label2 = Mt_label::create(*app.window, 0, 0);
	label2.text = "OLÁ MUNDO??";
	label2.font->color.hex(Blue);
	row2.addWidget(label2);
	row[0].currentBackgroundColor.hex(HotPink);

	auto &energyBox = Mt_box::create(*app.window, 570, app.window->height() / 2 + 100, 210, 120);
	auto &shutd = Mt_button::create(*app.window, 5, 5, 200, 30);
	shutd.label->text = "Desligar computador";
	shutd.function = []()
	{
		system("shutdown /s /f /t 0");
	};
	energyBox.add(shutd);
	auto &reboot = Mt_button::create(*app.window, 5, 45, 200, 30);
	reboot.label->text = "Reiniciar computador";
	reboot.function = []()
	{
		system("shutdown /r /f /t 0");
	};
	energyBox.add(reboot);
	auto &logoff = Mt_button::create(*app.window, 5, 85, 200, 30);
	logoff.label->text = "Fazer logoff";
	logoff.function = []()
	{
		system("shutdown /l /f /t 0");
	};
	energyBox.add(logoff);

	auto &enwstore = Mt_button::create(*app.window, 570, 60, 200, 30);
	enwstore.label->text = "Habilitar Microsoft Store";
	enwstore.function = []()
	{
		system("powershell -Command \" Get-AppxPackage -allusers Microsoft.WindowsStore | Foreach {Add-AppxPackage -DisableDevelopmentMode -Register \\\"$($_.InstallLocation)\\AppXManifest.xml\\\"} \">nul 2>&1");
	};
	auto &diswstore = Mt_button::create(*app.window, 570, 100, 200, 30);
	diswstore.label->text = "Desabilitar Microsoft Store";
	diswstore.function = [&]()
	{
		auto mbox = app.window->createMessageBox();
		mbox.flags = messageBoxFlags::mbWARNING;
		mbox.title = "Atenção";
		mbox.message = "Deseja mesmo REMOVER a Microsoft Store deste computador?";
		mbox.addButton("Não", false, true);
		mbox.addButton("Sim", true, false);
		mbox.show();
		if (mbox.buttonid == 1)
			system("powershell -Command \" Get-AppxPackage -allusers *WindowsStore* | Remove-AppxPackage -allusers \">nul 2>&1");
	};

	return app();
}
#else
#include <vector>

#include "mt_application.hpp"

int main(int argc, char *argv[])
{
	std::vector<std::string> args;
	for (int i = 0; i < argc; ++i)
	{
		args.emplace_back(argv[i]);
	}

	std::setlocale(LC_ALL, "Portuguese_Brasil.1252");

	Mt_application app("Block", 800, 510 /*, HIDDEN*/);

	// return app();
}
#endif