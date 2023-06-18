# Generated by Django 4.1.2 on 2022-11-22 17:48

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Admin',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('login', models.CharField(max_length=80)),
                ('password', models.CharField(max_length=80)),
            ],
        ),
        migrations.CreateModel(
            name='Categories',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('category_name', models.CharField(max_length=90)),
                ('approved', models.BooleanField(default=False)),
                ('category_of_id', models.ForeignKey(null=True, on_delete=django.db.models.deletion.CASCADE, to='index.categories')),
            ],
        ),
        migrations.CreateModel(
            name='Crop',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('crop_name', models.CharField(max_length=80)),
                ('description', models.CharField(max_length=80)),
                ('price', models.DecimalField(decimal_places=2, max_digits=8)),
                ('amount', models.IntegerField()),
                ('origin', models.CharField(max_length=80)),
                ('crop_year', models.IntegerField()),
                ('price_type', models.CharField(choices=[('perpc', 'per piece'), ('perkg', 'per kilogram')], default='perkg', max_length=5)),
                ('category', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='index.categories')),
            ],
        ),
        migrations.CreateModel(
            name='User',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('user_name', models.CharField(max_length=80)),
                ('email', models.CharField(max_length=80)),
                ('password', models.CharField(max_length=80)),
                ('mod', models.BooleanField(default=False)),
            ],
        ),
        migrations.CreateModel(
            name='Review',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('short_desc', models.CharField(max_length=80)),
                ('long_desc', models.CharField(max_length=200)),
                ('stars', models.IntegerField()),
                ('crop', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='index.crop')),
                ('reviewed_by', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='index.user')),
            ],
        ),
        migrations.CreateModel(
            name='Order',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('total_price', models.FloatField()),
                ('amount', models.IntegerField()),
                ('state', models.CharField(choices=[('pen', 'pending'), ('rej', 'rejected'), ('con', 'confirmed')], default='pending', max_length=10)),
                ('crop', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='index.crop')),
                ('farmer', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='farmer', to='index.user')),
                ('ordered_by', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='ordered_by', to='index.user')),
            ],
        ),
        migrations.CreateModel(
            name='Harvest',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('date', models.DateField()),
                ('place', models.CharField(max_length=80)),
                ('description', models.CharField(max_length=80)),
                ('max_occupancy', models.IntegerField(default=0)),
                ('current_occupancy', models.IntegerField(default=0)),
                ('attendees', models.ManyToManyField(to='index.user')),
                ('crop', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='index.crop')),
                ('farmer', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='harest_farmer', to='index.user')),
            ],
        ),
        migrations.AddField(
            model_name='crop',
            name='farmer',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='index.user'),
        ),
    ]