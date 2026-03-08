 Сушинський Микола  
ФЕ-41  
 ЗПАД

---

```
лабораторія_2/
├── .gitignore
├── README.md
├── requirements.txt
├── laba2part1.ipynb   # Частина 1 — VHI індекс України
└── laba2part2.ipynb   # Частина 2 — Electric Power Consumption
```

---


### Крок 1 — Створити віртуальне середовище (venv)

**Windows:**
```bash
python -m venv venv
```

### Крок 2 — Активувати venv

**Windows:**
```bash
venv\Scripts\activate
```

Після активації в терміналі з'явиться `(venv)` на початку рядка.

### Крок 3 — Встановити бібліотеки з вимоги.txt
```bash
pip install -r вимоги.txt
```

### Крок 4 — Запустити Jupyter Notebook
```bash
jupyter notebook
```

Браузер відкриє сторінку з файлами. Відкрий `laba2part1.ipynb` або `laba2part2.ipynb`.

### Крок 5 — Запустити всі комірки
```
Kernel → Restart Kernel and Run All Cells
```

---

## Бібліотеки

| Бібліотека | Використання |
|---|---|
| pandas | робота з DataFrame |
| numpy | числові операції |
| scipy | кореляція Пірсона та Спірмена |
| scikit-learn | нормалізація та стандартизація |
| jupyter | середовище виконання |
