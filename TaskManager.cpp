#include "TaskManager.h"
using namespace std;
TaskManager::TaskManager(): current_day(0) {}

void TaskManager::save_file() {
	ofstream out("baze.txt");
	if (!out.is_open()) {
		cout << "Ошибка открытия файла для записи." << endl;
		return;
	}
	// Записываем сотрудников в бд.
	out << employees.size() << endl;
	for (Employee* emp : employees) {
		out << emp->get_name() << " " << emp->get_hours_in_week() << endl;
	}
	// Записываем задачи в бд.
	out << tasks.size() << endl;
	for (Task* task : tasks) {
		out << task->get_id() << " ";
		out << task->get_description() << " ";
		out << task->get_workload_hours() << " ";
		out << task->get_remaining_workload() << " ";
		out << task->get_deadline_days() << " ";
		out << task->get_status() << " ";
		if (task->get_assigned_employee())
			out << task->get_assigned_employee()->get_name() << endl;
		else {
			out << "NONE" << endl;
		}
	}
	out.close();
}

void TaskManager::load_file() {
	ifstream in("baze.txt");
	if (!in.is_open()) {
		cout << "Ошибка открытия файла для чтения." << endl;
		return;
	}

	size_t emp_count;
	if (!(in >> emp_count)) {
		cout << "Файл пустой или поврежден (работники)." << endl;
		return;
	}
	in.ignore();

	employees.clear();
	tasks.clear();

	for (size_t i = 0; i < emp_count; ++i) {
		string name;
		int hours;
		in >> name >> hours;
		in.ignore();
		employees.push_back(new Employee(name, hours));
	}

	size_t task_count;
	if (!(in >> task_count)) {
		cout << "Файл пуст или поврежден (задачи)." << endl;
		return;
	}
	in.ignore();

	for (size_t i = 0; i < task_count; ++i) {
		int id;
		string desc;
		int hours, rem_hours, days;
		int status;
		string emp_name;

		if (!(in >> id >> desc >> hours >> rem_hours >> days >> status >> emp_name))
		{
			std::cout << "Ошибка формата задачи на строке " << (i + 1) << std::endl;
			break;
		}
		in.ignore();

		Employee* assigned = nullptr;
		if (emp_name != "NONE") {
			for (Employee* e : employees) {
				if (e->get_name() == emp_name) {
					assigned = e;
					break;
				}
			}
		}

		Task* t = new Task(id, desc, hours, days, status, assigned);
		tasks.push_back(t);
		if (assigned)
			assigned->assign_task(t);
	}
	in.close();
}

const std::vector<Employee*>& TaskManager::get_all_employees() { return employees; }
const std::vector<Task*>& TaskManager::get_all_tasks() { return tasks; }


void TaskManager::add_task(Task* task) {
	if (task != nullptr) {
		tasks.push_back(task);
		cout << "Задача " << task->get_id() << " добавлена." << endl;
	}
};
void TaskManager::remove_task(Task* task) {
	if (!task) {
		cout << "Ошибка: задача не найдена." << endl;
		return;
	}
	Employee* emp = task->get_assigned_employee();
	if (emp) {
		emp->remove_task(task);
		task->set_assigned_employee(nullptr);
	}
	for (size_t i = 0; i < tasks.size(); ++i) {
		if (tasks[i] == task) {
			cout << "Описание: " << task->get_description() << endl;
			tasks.erase(tasks.begin() + i);
			cout << "Задача удалена." << endl;
			return;
		}
	}
	cout << "Задача не найдена." << endl;
};
void TaskManager::search_task(int id) {
	for (Task* task : tasks) {
		if (task->get_id() == id) {
			cout << "Задача найдена: " << endl;
			cout << "ID: " << task->get_id() << endl;
			cout << "Описание: " << task->get_description() << endl;
			cout << "Часы: " << task->get_workload_hours() << endl;
			cout << "Дедлайн: " << task->get_deadline_days() << endl;
			cout << "Статус: " << (task->get_status() ? "Готова" : "Не готова") << endl;
			return;
		}
	}
	cout << "Задача с таким ID не найдена." << endl;
};
